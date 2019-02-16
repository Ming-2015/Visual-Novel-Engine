#include "MoveCommand.h"

MoveCommand::MoveCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "move")
	{
		LOGGER->Log("ShowCommand::MoveCommand", "Invalid Command Input");
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
			LOGGER->Log("MoveCommand", "Failed to convery x1 and y1 values to float values");
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
			LOGGER->Log("MoveCommand", "Failed to convery x2 and y2 values to float values");
		}
	}

	// OPTIONAL: Animation Time
	time = 0.0;// Default 1.5 seconds
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
	cout << endl << time << endl;

	// check flag validity
	if (time == 0)
	{
		cout << "DID WE CHECK THIS FLAG??";
		wait = false;
		time = 0;
		animationType = ANIMATION_NONE;
		//SEE IF SETTING POSITION = END POSITION WORKS
		x1 = x2;
		y1 = y2;
		//See if you can automatically move
	}
	if ((flag == "" || flag == "none") && time != 0)
	{
		wait = false;
		animationType = ANIMATION_MOVE;
	}
	if (flag == "wait" || flag == "Wait" || flag == "WAIT" && time != 0)
	{
		wait = true;
		animationType = ANIMATION_MOVE;
	}
	//WORK IN PROGRESS, ADD OTHER LATER

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
}

MoveCommand::~MoveCommand()
{

}

void MoveCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
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
	else
	{
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
	if (valid && time > 0)
	{
		if (animationType == ANIMATION_MOVE && (x1 != x2 || y1 != y2))
		{
			float yOffset = delta_t / time * yDiff;
			float xOffset = delta_t / time * xDiff;

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
	else
	{
		wait = false;
		x1 = x2;
		y1 = y2;
	}
}