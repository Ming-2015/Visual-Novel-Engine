#include "ZoomCommand.h"

ZoomCommand::ZoomCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "zoom")
	{
		LOGGER->Log("ZoomCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	currentScaleX = 1;
	currentScaleY = 1;

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
			LOGGER->Log("ZoomCommand", "Failed to convert x1 and y1 values to float values");
		}
	}

	scaleX = 0, scaleY = 0;
	if (args.size() > COLUMN_ARG6)
	{
		try
		{
			scaleX = stof(args[COLUMN_ARG5]);		// scale factor for X
			scaleY = stof(args[COLUMN_ARG6]);		// scale factor for Y
		}
		catch (exception e)
		{
			LOGGER->Log("ZoomCommand", "Failed to convert scale X and scale Y values to float values");
		}
	}

	// OPTIONAL: Animation Time
	time = 1.5; // Default 1.5 seconds
	if (args.size() > COLUMN_ARG7)
	{
		try {
			time = stof(args[COLUMN_ARG7]);			// time
		}
		catch (exception e)
		{
			LOGGER->Log("ZoomCommand", "Failed to convert time into float value");
		}
	}

	// check flag validity
	if (time == 0)
	{
		wait = false;
		time = 0;
		animationType = ANIMATION_NONE;
		currentScaleX = scaleX;
		currentScaleY = scaleY;
	}
	if ((flag == "" || flag == "none") && time != 0)
	{
		wait = false;
		animationType = ANIMATION_ZOOM;
	}
	if ((flag == "wait" || flag == "Wait" || flag == "WAIT") && time != 0)     //Move w/ Wait
	{
		wait = true;
		animationType = ANIMATION_ZOOM;
	}

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
		LOGGER->Log("ZoomCommand", "Invalid Object Type");
		return;
	}

	scaleDiffX = scaleX - currentScaleX;
	scaleDiffY = scaleY - currentScaleY;
}

ZoomCommand::~ZoomCommand()
{

}

void ZoomCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (valid)
		{
			if (objectType == OBJECT_CHARACTER)
			{
				//scriptLine->setCharacter(objectName, objectSubname, x1, y1);
				scriptLine->setCharacterZoom(objectName, objectSubname, currentScaleX, currentScaleY);
			}
			else if (objectType == OBJECT_BACKGROUND)
			{
				//scriptLine->setBackground(objectName, objectSubname, x1, y1);
				scriptLine->setBackgroundZoom(objectName, objectSubname, currentScaleX, currentScaleY);
			}

			if (animationType == ANIMATION_NONE && currentScaleX == scaleX && currentScaleY == scaleY)
			{
				done = true;
			}
		}
		else
		{
			done = true;
		}
	}
}

void ZoomCommand::skipUpdate()
{
	currentScaleX = scaleX;
	currentScaleY = scaleY;
	wait = false;
	done = true;
}

void ZoomCommand::update(float delta_t)
{
	if (valid && time > 0)
	{
		if (animationType == ANIMATION_ZOOM && (currentScaleX != scaleX || currentScaleY != scaleY))
		{
			float scaleXOffset = delta_t / time * scaleDiffX;
			float scaleYOffset = delta_t / time * scaleDiffY;

			currentScaleX += scaleXOffset;
			currentScaleY += scaleYOffset;

			if (scaleDiffX > 0)
			{
				if (currentScaleX >= scaleX)
				{
					currentScaleX = scaleX;
				}
			}
			if (scaleDiffX < 0)
			{
				if (currentScaleX <= scaleX)
				{
					currentScaleX = scaleX;
				}
			}
			if (scaleDiffY > 0)
			{
				if (currentScaleY >= scaleY)
				{
					currentScaleY = scaleY;
				}
			}
			if (scaleDiffY < 0)
			{
				if (currentScaleY <= scaleY)
				{
					currentScaleY = scaleY;
				}
			}
			if (currentScaleY == scaleY && currentScaleX == scaleX)
			{
				wait = false;
				done = true;
			}

		}
	}
	else
	{
		wait = false;
		currentScaleY = scaleY;
		currentScaleX = scaleX;
	}
}