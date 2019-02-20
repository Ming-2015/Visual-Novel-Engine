#include "DelayCommand.h"

DelayCommand::DelayCommand(std::vector<std::string> args)
	:ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "delay")
	{
		LOGGER->Log("DelayCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	// column 2 : object type character or background
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				// column 3 : flag

	// optional argument: time
	time = 0.5f;
	if (args.size() > COLUMN_ARG1)
	{
		try {
			time = std::stof(args[COLUMN_ARG1]); // column 8: time it should take to full finish the fade
		}
		catch (exception e)
		{
			LOGGER->Log("DelayCommand", "Failed to convert time into float");
		}
	}

	// check the flag validity
	if (flag == "" || flag == "none" || time <= 0)
	{
		animationType = ANIMATION_NONE;
	}
	else
	{
		LOGGER->Log("DelayCommand", "Invalid Flag");
		valid = false;
		return;
	}

	// check object type validity
	if (objectTypeName == "time" || objectTypeName == "t" || objectTypeName == "" )
	{
		wait = true;
		objectType = OBJECT_TIME;
	}
	else
	{
		valid = false;
		LOGGER->Log("DelayCommand", "Invalid Object Type");
		return;
	}
}

DelayCommand::~DelayCommand()
{
}

void DelayCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (animationType == ANIMATION_NONE)
		{
			if (finishedTime)
			{
				done = true;
				wait = false;
			}
		}
	}
	else
	{
		done = true;
		wait = false;
	}
}

void DelayCommand::skipUpdate()
{
	finishedTime = true;
}

void DelayCommand::update(float delta_t)
{
	if (valid)
	{
		currentTime += delta_t;
		if (currentTime > time)
		{
			finishedTime = true;
		}
	}
	else
	{
		finishedTime = true;
	}
}
