#include "EndCommand.h"

EndCommand::EndCommand(std::vector<std::string> args)
	:ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "end")
	{
		LOGGER->Log("EndCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_END;

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
			//LOGGER->Log("EndCommand", "Unable to parse time value");
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
		LOGGER->Log("EndCommand", "Invalid Flag");
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
		LOGGER->Log("EndCommand", "Invalid Object Type");
		return;
	}
}

EndCommand::~EndCommand()
{
}

EndCommand::EndCommand(ifstream & savefile)
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

void EndCommand::serialize(ofstream & savefile) const
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

void EndCommand::execute(ScriptLine * scriptLine)
{
	if (!valid) return;

	if (objectType == OBJECT_CINEMATIC)
	{
		if (!initialized)
		{
			initialized = true;

			if (scriptLine->isDisplayingCinematicBars())
			{
				maxHeight = scriptLine->getCinematicBarsHeight();
				currentHeight = maxHeight;
			}
			else
			{
				skipUpdate();
			}
		}

		if (currentHeight <= 0)
		{
			scriptLine->displayCinematicBars(false);
		}

		scriptLine->setCinematicBarsHeight(currentHeight);
	}
}

void EndCommand::skipUpdate()
{
	if (!valid) return;

	if (objectType == OBJECT_CINEMATIC)
	{
		currentHeight = 0;
		done = true;
		wait = false;
	}
}

void EndCommand::update(float delta_t)
{
	if (!valid) return;

	if (objectType == OBJECT_CINEMATIC)
	{
		if (!initialized) return;
		
		float increment = delta_t / time * maxHeight;
		currentHeight -= increment;

		// done with moving the bars
		if (currentHeight <= 0)
		{
			skipUpdate();
		}
	}
}
