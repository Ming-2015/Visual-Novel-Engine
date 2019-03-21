#include "StopLoopCommand.h"

StopLoopCommand::StopLoopCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "stoploop")
	{
		LOGGER->Log("StopLoopCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_STOPLOOP;

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);
	loopName = args[COLUMN_ARG1];
	flagExpression = args[COLUMN_ARG2];

	// check object type
	if (objectTypeName == "loop" || objectTypeName == "l" || objectTypeName == "")
	{
		objectType = OBJECT_LOOP;
	}
	else
	{
		LOGGER->Log("StopLoopCommand", "Invalid Object Type");
		valid = false;
		return;
	}

	// check flag type
	if (flag == "none" || flag == "n" || flag == "n")
	{
		flagType = FLAG_NONE;
	}
	else if (flag == "flag" || flag == "f")
	{
		flagType = FLAG_PARSE;
	}
	else
	{
		LOGGER->Log("StopLoopCommand", "Invalid Flag Argument");
		valid = false;
		return;
	}

	wait = true;
}

StopLoopCommand::~StopLoopCommand()
{
}

StopLoopCommand::StopLoopCommand(ifstream & savefile)
	: ScriptCommand(savefile)
{

	objectTypeName = UTILITY->readFromBinaryFile(savefile);
	flag = UTILITY->readFromBinaryFile(savefile);
	loopName = UTILITY->readFromBinaryFile(savefile);
	flagExpression = UTILITY->readFromBinaryFile(savefile);

	savefile.read(reinterpret_cast<char *> (&flagType), sizeof(flagType));
	savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
}

void StopLoopCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);
	UTILITY->writeToBinaryFile(savefile, loopName);
	UTILITY->writeToBinaryFile(savefile, flagExpression);

	savefile.write(reinterpret_cast<const char *> (&flagType), sizeof(flagType));
	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
}

void StopLoopCommand::execute(ScriptLine * scriptLine)
{
	if (!valid) return;

	// check to see if we should be stopping the loop
	bool shouldStopLoop = false;
	if (flagType == FLAG_NONE)
	{
		shouldStopLoop = true;
	}
	else if (flagType == FLAG_PARSE)
	{
		shouldStopLoop = UTILITY->evaluateFlagExpression(scriptLine->userFlags, flagExpression);
	}

	// stopping the loop
	if (shouldStopLoop)
	{
		scriptLine->removeLoop(loopName);
	}

	done = true;
}

void StopLoopCommand::skipUpdate()
{
}

void StopLoopCommand::update(float delta_t)
{
}
