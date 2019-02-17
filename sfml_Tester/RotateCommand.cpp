#include "RotateCommand.h"

RotateCommand::RotateCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "rotate")
	{
		LOGGER->Log("RotateCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);		// object type: char / bg
	flag = UTILITY->toLower(args[COLUMN_FLAG]);					// flag

	objectName = UTILITY->toLower(args[COLUMN_ARG1]);			// object name
	objectSubname = UTILITY->toLower(args[COLUMN_ARG2]);		// object sub name

	clockwise = true;
	if (args.size() > COLUMN_ARG3)
	{
		try
		{
			whichWay = args[COLUMN_ARG3];
			if (whichWay == "clockwise" || whichWay == "clock" || whichWay == "cw" )
			{
				clockwise = true;
			}
			else if (whichWay == "counter" || whichWay == "counterclockwise" || whichWay == "counterclock" || whichWay == "ccw" )
			{
				clockwise = false;
			}
		}
		catch (exception e)
		{
			LOGGER->Log("RotateCommand", "Failed to ARG5 store as string");
		}
	}

	finalDegree = 0;
	if (args.size() > COLUMN_ARG4)
	{
		try
		{
			finalDegree = stof(args[COLUMN_ARG4]);
		}
		catch (exception e)
		{
			LOGGER->Log("RotateCommand", "Failed to convert degree to float values");
		}
	}

	time = 0.0;
	if (args.size() > COLUMN_ARG5)
	{
		try {
			time = stof(args[COLUMN_ARG5]);	
		}
		catch (exception e)
		{
			LOGGER->Log("MoveCommand", "Failed to convert time into float value");
		}
	}

	if (time <= 0)		//No Animation
	{
		wait = false;
		relative = false;
		time = 0;
		animationType = ANIMATION_NONE;
		currentRotate = finalDegree;
	}
	else if (flag == "" || flag == "none")		//Rotate w/o wait
	{
		wait = false;
		animationType = ANIMATION_ROTATE;
		relative = false;
	}
	else if (flag == "wait" || flag == "w")
	{
		wait = true;
		animationType = ANIMATION_ROTATE;
		relative = false;
	}
	else if (flag == "relative" || flag == "r")
	{
		wait = false;
		relative = true;
		animationType = ANIMATION_ROTATE;
	}
	else if (flag == "relativewait" || flag == "rw" )
	{
		wait = true;
		relative = true;
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
	tempAngle = 0;
}

RotateCommand::~RotateCommand()
{

}

void RotateCommand::execute(ScriptLine * scriptLine)
{
	if (valid && relative == false)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->setCharacterRotation(objectName, objectSubname, clockwise, currentRotate);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setBackgroundRotation(objectName, objectSubname, clockwise, currentRotate);
		}

		if (animationType == ANIMATION_NONE || stopRotate)
		{
			done = true;
		}
	}
	else if (valid && relative == true)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->setCharacterRotationRel(objectName, objectSubname, clockwise, currentRotate);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setBackgroundRotationRel(objectName, objectSubname, clockwise, currentRotate);
		}

		if (animationType == ANIMATION_NONE || stopRotate)
		{
			done = true;
		}
	}
	else
	{
		LOGGER->Log("RotateCommand", "Invalid Rotate command detected");
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
	if (valid && relative == false)
	{
		if (animationType == ANIMATION_ROTATE && currentRotate < finalDegree)
		{
			float degreeOffset = delta_t / time * angleDiff;
			currentRotate += degreeOffset;

			if (currentRotate >= finalDegree)
			{
				stopRotate = true;
			}
		}
	}
	else if (valid && relative == true)
	{
		if (animationType == ANIMATION_ROTATE && tempAngle < finalDegree)
		{
			float degreeOffset = delta_t / time * angleDiff;
			tempAngle += degreeOffset;
			currentRotate = degreeOffset;

			if (tempAngle >= finalDegree)
			{
				stopRotate = true;
			}
		}
	}
}