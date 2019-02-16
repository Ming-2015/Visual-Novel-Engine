#include "RotateCommand.h"

RotateCommand::RotateCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "rotate")
	{
		LOGGER->Log("ShowCommand::RotateCommand", "Invalid Command Input");
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
			LOGGER->Log("RotateCommand", "Failed to convert x1 and y1 values to float values");
		}
	}

	if (args.size() > COLUMN_ARG5)
	{
		try
		{
			whichWay = args[COLUMN_ARG5];
			if (whichWay == "clockwise" || whichWay == "Clockwise" || whichWay == "clock" || whichWay == "Clock")
				bool clockwise = true;
			else if (whichWay == "counter" || whichWay == "counterclockwise" || whichWay == "Counter" || whichWay == "counterclock" || whichWay == "Counterclock" || whichWay == "Counterclockwise")
				bool clockwise = false;
		}
		catch (exception e)
		{
			LOGGER->Log("RotateCommand", "Failed to ARG5 store as string");
		}
	}

	if (args.size() > COLUMN_ARG6)
	{
		try
		{
			finalDegree = stof(args[COLUMN_ARG6]);
		}
		catch (exception e)
		{
			LOGGER->Log("RotateCommand", "Failed to convert degree to float values");
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

	if (time == 0)		//No Animation
	{
		wait = false;
		time = 0;
		animationType = ANIMATION_NONE;
		currentRotate = finalDegree;
	}
	if ((flag == "" || flag == "none") && time != 0)		//Rotate w/o wait
	{
		wait = false;
		animationType = ANIMATION_ROTATE;
	}
	if ((flag == "wait" || flag == "Wait" || flag == "WAIT") && time != 0)
	{
		wait = true;
		animationType = ANIMATION_ROTATE;
	}

	//object type validity
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
		LOGGER->Log("RotateCommand", "Invalid Object Type");
		return;
	}

	angleDiff = finalDegree - currentRotate;
}

RotateCommand::~RotateCommand()
{

}

void RotateCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->setCharacterRotation(objectName, objectSubname, clockwise, currentRotate);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setBackgroundRotation(objectName, objectSubname, clockwise, currentRotate);
		}

		if (animationType == ANIMATION_NONE && currentRotate == finalDegree)
		{
			done = true;
		}
	}
	else
	{
		done = true;
	}
}

void RotateCommand::skipUpdate()
{
	currentRotate = finalDegree;
	wait = false;
	done = true;
}

void RotateCommand::update(float delta_t)
{
	if (valid && time > 0)
	{
		if (animationType == ANIMATION_ROTATE && fabs(currentRotate) < finalDegree)
		{
			float degreeOffset = delta_t / time * angleDiff;
			if (clockwise == true)
			{
				currentRotate += degreeOffset;
				if (fabs(currentRotate) >= finalDegree)
				{
					currentRotate = finalDegree;
				}
			}
			if (clockwise == false)
			{
				currentRotate -= degreeOffset;
				if (fabs(currentRotate) >= finalDegree)
				{
					currentRotate = -finalDegree;
				}
			}
			if (fabs(currentRotate) == finalDegree)
			{
				wait = false;
				done = true;
			}
		}
	}
}