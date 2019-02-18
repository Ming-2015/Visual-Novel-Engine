#include "MoveCommand.h"

MoveCommand::MoveCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "move")
	{
		LOGGER->Log("MoveCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);		// object type: char / bg
	flag = UTILITY->toLower(args[COLUMN_FLAG]);					// flag

	objectName = UTILITY->toLower(args[COLUMN_ARG1]);			// object name
	objectSubname = UTILITY->toLower(args[COLUMN_ARG2]);		// object sub name

	// Optional if begin x and y = 0
	x1 = 0, y1 = 0;
	if (args.size() > COLUMN_ARG4)
	{
		try 
		{
			x1 = stof(args[COLUMN_ARG3]);		// Beginning x position
			y1 = stof(args[COLUMN_ARG4]);		// Beginning y position
		}
		catch (exception e)
		{
			LOGGER->Log("MoveCommand", "Failed to convert x1 and y1 values to float values");
		}
	}

	// Optional if end x and y = 0
	x2 = 0, y2 = 0;
	if (args.size() > COLUMN_ARG6)
	{
		try
		{
			x2 = stof(args[COLUMN_ARG5]);		// End x position
			y2 = stof(args[COLUMN_ARG6]);		// End y position
		}
		catch (exception e)
		{
			LOGGER->Log("MoveCommand", "Failed to convert x2 and y2 values to float values");
		}
	}

	// OPTIONAL: Animation Time
	time = 1.5;// Default 1.5 seconds
	if (args.size() > COLUMN_ARG7)
	{
		try {
			time = stof(args[COLUMN_ARG7]);			// time
		}
		catch (exception e)
		{
			LOGGER->Log("MoveCommand", "Failed to convert time into float value");
		}
	}

	// check flag validity
	if (time <= 0)
	{
		wait = false;
		relative = false;
		time = 0;
		animationType = ANIMATION_NONE;
		//SEE IF SETTING POSITION = END POSITION WORKS
		x1 = x2;
		y1 = y2;
		//See if you can automatically move
	}
	if ((flag == "" || flag == "none") && time != 0)	//Move w/o Wait
	{
		wait = false;
		relative = false;
		animationType = ANIMATION_MOVE;
	}
	else if (flag == "wait" || flag == "w")
	{
		wait = true;
		animationType = ANIMATION_MOVE;
		relative = false;
	}
	else if (flag == "relative" || flag == "r")
	{
		wait = false;
		relative = true;
		animationType = ANIMATION_MOVE;
	}
	else if (flag == "relativewait" || flag == "rw")
	{
		wait = true;
		relative = true;
		animationType = ANIMATION_MOVE;
	}
	//WORK IN PROGRESS, ADD ACCELERATE

	// object type validity
	if (objectTypeName == "character" || objectTypeName == "c" || objectTypeName == "char")
	{
		objectType = OBJECT_CHARACTER;
	}
	else if (objectTypeName == "background" || objectTypeName == "b" || objectTypeName == "bg")
	{
		objectType = OBJECT_BACKGROUND;
	}
	else
	{
		valid = false;
		LOGGER->Log("MoveCommand", "Invalid Object Type");
		return;
	}

	xDiff = x2 - x1;
	yDiff = y2 - y1;
	xOffset = 0;
	yOffset = 0;
}

MoveCommand::~MoveCommand()
{

}

void MoveCommand::execute(ScriptLine * scriptLine)
{
	if (valid && relative == false)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->setCharacter(objectName, objectSubname, x1, y1);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setBackground(objectName, objectSubname, x1, y1);
		}

		if (animationType == ANIMATION_NONE && x1 == x2 && y1 == y2)
		{
			done = true;
		}
	}
	else if (valid && relative == true)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->moveCharacterRel(objectName, objectSubname, xOffset, yOffset);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->moveBackgroundRel(objectName, objectSubname, xOffset, yOffset);
		}

		if (animationType == ANIMATION_NONE || stopMove)
		{
			done = true;
		}
	}
	else
	{
		LOGGER->Log("RotateCommand", "Invalid Move command detected");
		done = true;
	}
}

void MoveCommand::skipUpdate()
{
	//Need to clean up position variables?
	x1 = x2;
	y1 = y2;
	wait = false;
	done = true;
}

void MoveCommand::update(float delta_t)
{
	if (valid && relative == false)
	{
		if (animationType == ANIMATION_MOVE && (x1 != x2 || y1 != y2))
		{
			yOffset = delta_t / time * yDiff;
			xOffset = delta_t / time * xDiff;

			x1 += xOffset;
			y1 += yOffset;
			if (xDiff > 0)
			{
				if (x1 >= x2)
					x1 = x2;
			}
			else if (xDiff < 0)
			{
				if (x1 <= x2)
					x1 = x2;
			}
			
			if (yDiff > 0)
			{
				if (y1 >= y2)
					y1 = y2;
			}
			if (yDiff < 0)
			{
				if (y1 <= y2)
					y1 = y2;
			}
			if (x1 == x2 && y1 == y2)
			{
				wait = false;
				done = true;
				stopMove = true;
			}
		}

		
		/*if (animationType == ANIMATION_MOVE && (x1 < x2 || y1 < y2))
		{
			float moveOffset = delta_t / time * 255.f;

			x1 += moveOffset;
			y1 += moveOffset;
			if (x1 >= x2) 
			{
				x1 = x2;
			}
			if (y1 >= y2)
			{
				y1 = y2;
			}
			if (x1 == x2 && y1 == y2)
			{
				wait = false;
			}
		}
		if (animationType == ANIMATION_MOVE && (x1 > x2 || y1 < y2))
		{
			float moveOffset = delta_t / time * 255.f;

			x1 -= moveOffset;
			y1 += moveOffset;
			if (x1 <= x2)
			{
				x1 = x2;
			}
			if (y1 >= y2)
			{
				y1 = y2;
			}
			if (x1 == x2 && y1 == y2)
			{
				wait = false;
			}
		}
		if (animationType == ANIMATION_MOVE && (x1 < x2 || y1 > y2))
		{
			float moveOffset = delta_t / time * 255.f;

			x1 += moveOffset;
			y1 -= moveOffset;
			if (x1 >= x2)
			{
				x1 = x2;
			}
			if (y1 <= y2)
			{
				y1 = y2;
			}
			if (x1 == x2 && y1 == y2)
			{
				wait = false;
			}
		}
		if (animationType == ANIMATION_MOVE && (x1 > x2 || y1 > y2))
		{
			float moveOffset = delta_t / time * 255.f;

			x1 -= moveOffset;
			y1 -= moveOffset;
			if (x1 <= x2)
			{
				x1 = x2;
			}
			if (y1 <= y2)
			{
				y1 = y2;
			}
			if (x1 == x2 && y1 == y2)
			{
				wait = false;
			}
		}*/
	}
	else if (valid && relative == true)
	{
		if (animationType == ANIMATION_MOVE && (x1 != x2 || y1 != y2))
		{
			yOffset = delta_t / time * yDiff;
			xOffset = delta_t / time * xDiff;
			x1 += xOffset;
			y1 += yOffset;
		}
		if (xDiff > 0)
		{
			if (x1 >= x2)
				x1 = x2;
		}
		else if (xDiff < 0)
		{
			if (x1 <= x2)
				x1 = x2;
		}

		if (yDiff > 0)
		{
			if (y1 >= y2)
				y1 = y2;
		}
		if (yDiff < 0)
		{
			if (y1 <= y2)
				y1 = y2;
		}
		if (x1 == x2 && y1 == y2)
		{
			wait = false;
			done = true;
			stopMove = true;
		}
	}
	else
	{
		wait = false;
		x1 = x2;
		y1 = y2;
	}
}