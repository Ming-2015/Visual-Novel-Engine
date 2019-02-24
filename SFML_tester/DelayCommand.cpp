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
	commandType = COMMAND_DELAY;

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
			//LOGGER->Log("DelayCommand", "Failed to convert time into float");
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

DelayCommand::DelayCommand(ifstream & savefile)
	:ScriptCommand(savefile)
{
	try {
		objectTypeName = UTILITY->readFromBinaryFile(savefile);
		flag = UTILITY->readFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *> (&time), sizeof(time));
		savefile.read(reinterpret_cast<char *> (&currentTime), sizeof(currentTime));
		savefile.read(reinterpret_cast<char *> (&finishedTime), sizeof(finishedTime));

		savefile.read(reinterpret_cast<char *> (&animationType), sizeof(animationType));

		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
	}
	catch (exception e)
	{
		LOGGER->Log("DelayCommand", "Unable to read delay command");
		valid = false;
		return;
	}
}

void DelayCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);

	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));
	savefile.write(reinterpret_cast<const char *> (&currentTime), sizeof(currentTime));
	savefile.write(reinterpret_cast<const char *> (&finishedTime), sizeof(finishedTime));

	savefile.write(reinterpret_cast<const char *> (&animationType), sizeof(animationType));

	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
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
