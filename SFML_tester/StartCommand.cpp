#include "StartCommand.h"

StartCommand::StartCommand(std::vector<std::string> args)
	:ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "start")
	{
		LOGGER->Log("StartCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_START;

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);

	// OPTIONAL: Animation Time
	time = 1.f;
	if (args.size() > COLUMN_ARG1)
	{
		try {
			time = stof(args[COLUMN_ARG1]);
		}
		catch (exception e) {
			//LOGGER->Log("StartCommand", "Unable to parse time value");
		}
	}

	// OPTIONAL: Animation Time
	maxHeight = 100.f;
	if (args.size() > COLUMN_ARG2)
	{
		try {
			maxHeight = stof(args[COLUMN_ARG2]);
		}
		catch (exception e) {
			//LOGGER->Log("StartCommand", "Unable to parse time value");
		}
	}

	// check the flag validity
	wait = true; // need to wait 1 iteration to reach execute
	if (flag == "" || flag == "none" || flag == "n" || time <= 0)
	{
		animationType = ANIMATION_NONE;
		wait = false;
	}
	else if (flag == "wait" || flag == "w")
	{
		animationType = ANIMATION_WAIT;
		wait = true;
	}
	else
	{
		LOGGER->Log("StartCommand", "Invalid Flag");
		valid = false;
		return;
	}

	// check object type validity
	if (objectTypeName == "cinematic" || objectTypeName == "c" || objectTypeName == "cinematics")
	{
		objectType = OBJECT_CINEMATIC;
	}
	else
	{
		valid = false;
		LOGGER->Log("StartCommand", "Invalid Object Type");
		return;
	}
}

StartCommand::~StartCommand()
{
}

StartCommand::StartCommand(ifstream & savefile)
	:ScriptCommand(savefile)
{

	objectTypeName = UTILITY->readFromBinaryFile(savefile);
	flag = UTILITY->readFromBinaryFile(savefile);

	savefile.read(reinterpret_cast<char *> (&initialized), sizeof(initialized));
	savefile.read(reinterpret_cast<char *> (&currentHeight), sizeof(currentHeight));

	savefile.read(reinterpret_cast<char *> (&maxHeight), sizeof(maxHeight));
	savefile.read(reinterpret_cast<char *> (&time), sizeof(time));

	savefile.read(reinterpret_cast<char *> (&animationType), sizeof(animationType));
	savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
}

void StartCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);

	savefile.write(reinterpret_cast<const char *> (&initialized), sizeof(initialized));
	savefile.write(reinterpret_cast<const char *> (&currentHeight), sizeof(currentHeight));

	savefile.write(reinterpret_cast<const char *> (&maxHeight), sizeof(maxHeight));
	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));

	savefile.write(reinterpret_cast<const char *> (&animationType), sizeof(animationType));
	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
}

void StartCommand::execute(ScriptLine * scriptLine)
{
	if (!valid) return;

	if (objectType == OBJECT_CINEMATIC)
	{
		if (!initialized)
		{
			initialized = true;

			if (scriptLine->isDisplayingCinematicBars())
			{
				currentHeight += scriptLine->getCinematicBarsHeight();
				if (currentHeight >= maxHeight)
				{
					skipUpdate();
				}
			}
			else
			{
				scriptLine->displayCinematicBars(true);
			}
		}

		scriptLine->setCinematicBarsHeight(currentHeight);
	}
}

void StartCommand::skipUpdate()
{
	if (!valid) return;

	if (objectType == OBJECT_CINEMATIC)
	{
		currentHeight = maxHeight;
		done = true;
		wait = false;
	}
}

void StartCommand::update(float delta_t)
{
	if (!valid) return;

	if (objectType == OBJECT_CINEMATIC)
	{
		float increment = delta_t / time * maxHeight;
		currentHeight += increment;

		// done with moving the bars
		if (currentHeight >= maxHeight)
		{
			skipUpdate();
		}
	}
}
