#include "JumpCommand.h"

JumpCommand::JumpCommand(std::vector<std::string> args)
	:ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "jump")
	{
		LOGGER->Log("JumpCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	// column 2 : object type character or background
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				// column 3 : flag

	// check the flag validity
	if (flag == "" || flag == "none" || flag == "always" || flag == "n" || flag == "a" )
	{
		conditionType = CONDITION_NONE;
	}
	else if (flag == "flag" || flag == "f" || flag == "conditional" || flag == "c")
	{
		conditionType = CONDITION_FLAG;
	}
	else
	{
		LOGGER->Log("JumpCommand", "Invalid Flag");
		valid = false;
		return;
	}

	// check object type validity
	if (objectTypeName == "file" || objectTypeName == "f" || objectTypeName == "")
	{
		objectType = OBJECT_FILE;
	}
	else if (objectTypeName == "anchor" || objectTypeName == "a")
	{
		objectType == OBJECT_ANCHOR;
	}
	else
	{
		valid = false;
		LOGGER->Log("JumpCommand", "Invalid Object Type");
		return;
	}

	// flag expression
	if (conditionType == CONDITION_FLAG)
	{
		flagExpression = args[COLUMN_ARG1];
	}

	// filename
	if (conditionType == CONDITION_NONE)
	{
		filename = args[COLUMN_ARG1];
	}
	else if (conditionType == CONDITION_FLAG)
	{
		filename = args[COLUMN_ARG2];
	}

	// anchor
	if (objectType == OBJECT_ANCHOR)
	{
		if (conditionType == CONDITION_NONE)
		{
			anchor = args[COLUMN_ARG2];
		}
		else if (conditionType == CONDITION_FLAG)
		{
			anchor = args[COLUMN_ARG3];
		}
	}

	wait = true;
}

JumpCommand::~JumpCommand()
{
}

void JumpCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		bool shouldJump = false;
		if (conditionType == CONDITION_NONE)
		{
			shouldJump = true;
		}
		else if (conditionType == CONDITION_FLAG)
		{
			shouldJump = UTILITY->evaluateFlagExpression(GLOBAL->userFlags, flagExpression);
		}

		if (shouldJump)
		{
			if (objectType == OBJECT_FILE)
			{
				scriptLine->readNewFile(GLOBAL->ResourceRoot + filename);
			}
			else if (objectType == OBJECT_ANCHOR)
			{
				scriptLine->readNewFileToAnchor(GLOBAL->ResourceRoot + filename, anchor);
			}
		}
	}

	wait = false;
	done = true;
}

void JumpCommand::skipUpdate()
{
}

void JumpCommand::update(float delta_t)
{
}
