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
	startX = 0, startY = 0;
	if (args.size() > COLUMN_ARG4)
	{
		try 
		{
			startX = stof(args[COLUMN_ARG3]);		// Beginning x position
			startY = stof(args[COLUMN_ARG4]);		// Beginning y position
		}
		catch (exception e)
		{
			LOGGER->Log("MoveCommand", "Failed to convert startX and startY values to float values");
		}
	}

	// Optional if end x and y = 0
	endX = 0, endY = 0;
	if (args.size() > COLUMN_ARG6)
	{
		try
		{
			endX = stof(args[COLUMN_ARG5]);		// End x position
			endY = stof(args[COLUMN_ARG6]);		// End y position
		}
		catch (exception e)
		{
			LOGGER->Log("MoveCommand", "Failed to convert endX and endY values to float values");
		}
	}

	// OPTIONAL: Animation Time
	time = .5;
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
		startX = endX;
		startY = endY;
		//See if you can automatically move
	}
	if (flag == "" || flag == "none")	//Move w/o Wait
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

	xDiff = endX - startX;
	yDiff = endY - startY;
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
			scriptLine->setCharacter(objectName, objectSubname, startX, startY);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setBackground(objectName, objectSubname, startX, startY);
		}

		if (animationType == ANIMATION_NONE || stopMove)
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
		LOGGER->Log("MoveCommand", "Invalid Move command detected");
		done = true;
	}
}

void MoveCommand::skipUpdate()
{
	//Need to clean up position variables?
	if (!relative)
	{
		startX = endX;
		startY = endY;
	}
	else
	{
		xOffset = endX - startX;
		yOffset = endY - startY;
		startX = endX;
		startY = endY;
	}

	stopMove = true;
	wait = false;
	done = true;
}

void MoveCommand::update(float delta_t)
{
	if (valid && relative == false)
	{
		if (animationType == ANIMATION_MOVE)
		{
			if (!doneX)
			{
				xOffset = delta_t / time * xDiff;
				startX += xOffset;

				if (xDiff > 0 && startX >= endX)	// in case endX is greater than startX originally
				{
					doneX = true;
					startX = endX;
				}

				else if (xDiff <= 0 && startX <= endX)
				{
					startX = endX;
					doneX = true;
				}
			}

			if (!doneY)
			{
				yOffset = delta_t / time * yDiff;
				startY += yOffset;

				if (yDiff > 0 && startY >= endY)
				{
					startY = endY;
					doneY = true;
				}
				else if (yDiff <= 0 && startY <= endY)
				{
					doneY = true;
					startY = endY;
				}
			}

			if (doneX && doneY)
			{
				wait = false;
				done = true;
				stopMove = true;
			}

		}
	}
	else if (valid && relative == true)
	{
		if (animationType == ANIMATION_MOVE)
		{
			if (!doneX)
			{
				xOffset = delta_t / time * xDiff;
				startX += xOffset;

				if (xDiff > 0 && startX >= endX)	// in case endX is greater than startX originally
				{
					doneX = true;
					xOffset = xOffset - (startX - endX);
					startX = endX;
				}

				else if (xDiff <= 0 && startX <= endX)
				{
					doneX = true;
					xOffset = xOffset - (startX - endX);
					startX = endX;
				}
			}

			if (!doneY)
			{
				yOffset = delta_t / time * yDiff;
				startY += yOffset;

				if (yDiff > 0 && startY >= endY)
				{
					doneY = true;
					yOffset = yOffset - (startY - endY);
					startY = endY;
					
				}
				else if (yDiff <= 0 && startY <= endY)
				{
					doneY = true;
					yOffset = yOffset - (startY - endY);
					startY = endY;
				}
			}

			if (doneX && doneY)
			{
				wait = false;
				done = true;
				stopMove = true;
			}
		}
	}
	else
	{
		if (!relative)
		{
			startX = endX;
			startY = endY;
		}
		else
		{
			xOffset = endX - startX;
			yOffset = endY - startY;
			startX = endX;
			startY = endY;
		}
		done = true;
		wait = false;
		stopMove = true;
		doneX = true;
		doneY = true;
	}
}