#include "PlayCommand.h"

PlayCommand::PlayCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "play")
	{
		LOGGER->Log("PlayCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_PLAY;

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	// column 2 : object type character or background
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				// column 3 : flag

	objectName = args[COLUMN_ARG1];		// column 4 : object name
	objectSubname = args[COLUMN_ARG2];	// column 5 : object sub name

	// optional argument: time
	time = 1.5f;
	if (args.size() > COLUMN_ARG3)
	{
		try {
			time = std::stof(args[COLUMN_ARG3]); // column 8: time it should take to full finish the fade
		}
		catch (exception e)
		{
			LOGGER->Log("PlayCommand", "Failed to convert time into float");
		}
	}

	finalVolume = 1.0f;
	if (args.size() > COLUMN_ARG4)
	{
		try {
			finalVolume = std::stof(args[COLUMN_ARG4]); // column 8: time it should take to full finish the fade
		}
		catch (exception e)
		{
			LOGGER->Log("PlayCommand", "Failed to convert volume into float");
		}
	}

	// check the flag validity
	if (flag == "" || flag == "none" || time <= 0)
	{
		wait = false;
		flagType = FLAG_NONE;
		currentVolume = finalVolume;
		time = 0;
	}
	else if (flag == "fadeinwait" || flag == "fw")
	{
		wait = true;
		flagType = FLAG_FADEIN;
	}
	else if (flag == "fadein" || flag == "f")
	{
		wait = false;
		flagType = FLAG_FADEIN;
	}
	else
	{
		LOGGER->Log("PlayCommand", "Invalid Flag");
		valid = false;
		return;
	}

	// check object type validity
	if (objectTypeName == "bgm" || objectTypeName == "music" || objectTypeName == "background music" || objectTypeName == "b")
	{
		objectType = OBJECT_BGM;
		repeat = true;
		clearPrev = true;
	}
	else if (objectTypeName == "voice" || objectTypeName == "v")
	{
		objectType = OBJECT_VOICE;
		repeat = false;
		clearPrev = true;
	}
	else if (objectTypeName == "sfx" || objectTypeName == "sound effect" || objectTypeName == "" || objectTypeName == "sound" || objectTypeName == "s")
	{
		objectType = OBJECT_SFX;
		repeat = false;
		clearPrev = false;
	}
	else
	{
		valid = false;
		LOGGER->Log("PlayCommand", "Invalid Object Type");
		return;
	}
}

PlayCommand::~PlayCommand()
{
}

PlayCommand::PlayCommand(ifstream & savefile)
	:ScriptCommand(savefile)
{
	try
	{
		objectTypeName = UTILITY->readFromBinaryFile(savefile);
		flag = UTILITY->readFromBinaryFile(savefile);
		objectName = UTILITY->readFromBinaryFile(savefile);
		objectSubname = UTILITY->readFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *> (&time), sizeof(time));
		savefile.read(reinterpret_cast<char *> (&finalVolume), sizeof(finalVolume));
		savefile.read(reinterpret_cast<char *> (&currentVolume), sizeof(currentVolume));

		savefile.read(reinterpret_cast<char *> (&flagType), sizeof(flagType));
		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
		savefile.read(reinterpret_cast<char *> (&repeat), sizeof(repeat));

		savefile.read(reinterpret_cast<char *> (&clearPrev), sizeof(clearPrev));
		savefile.read(reinterpret_cast<char *> (&finishedAction), sizeof(finishedAction));
		savefile.read(reinterpret_cast<char *> (&played), sizeof(played));
	}
	catch (exception e)
	{
		LOGGER->Log("PlayCommand", "Unable to read Play command from savedata");
		valid = false;
		return;
	}
	played = false;
}

void PlayCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);
	UTILITY->writeToBinaryFile(savefile, objectName);
	UTILITY->writeToBinaryFile(savefile, objectSubname);

	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));
	savefile.write(reinterpret_cast<const char *> (&finalVolume), sizeof(finalVolume));
	savefile.write(reinterpret_cast<const char *> (&currentVolume), sizeof(currentVolume));

	savefile.write(reinterpret_cast<const char *> (&flagType), sizeof(flagType));
	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
	savefile.write(reinterpret_cast<const char *> (&repeat), sizeof(repeat));

	savefile.write(reinterpret_cast<const char *> (&clearPrev), sizeof(clearPrev));
	savefile.write(reinterpret_cast<const char *> (&finishedAction), sizeof(finishedAction));
	savefile.write(reinterpret_cast<const char *> (&played), sizeof(played));
}

void PlayCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (!played)
		{
			played = true;
			switch (objectType)
			{
				case OBJECT_BGM:
				{
					music = scriptLine->setBgm(objectName, objectSubname, clearPrev, repeat, currentVolume);
					break;
				}
				case OBJECT_SFX:
				{
					music = scriptLine->setSfx(objectName, objectSubname, clearPrev, repeat, currentVolume);
					break;
				}
				case OBJECT_VOICE:
				{
					music = scriptLine->setVoice(objectName, objectSubname, clearPrev, repeat, currentVolume);
					break;
				}
			}
		}

		if (flagType == FLAG_NONE)
		{
			done = true;
			wait = false;
		}
		else if (flagType == FLAG_FADEIN)
		{
			switch (objectType)
			{
				case OBJECT_BGM:
				{
					scriptLine->setBgmVolume(music, currentVolume);
					break;
				}
				case OBJECT_SFX:
				{
					scriptLine->setSfxVolume(music, currentVolume);
					break;
				}
				case OBJECT_VOICE:
				{
					scriptLine->setVoiceVolume(music, currentVolume);
					break;
				}
			}

			if (finishedAction)
			{
				done = true;
				wait = false;
			}
		}
	}
	else
	{
		LOGGER->Log("PlayCommand", "Attempting to execute invalid command");
		done = true;
	}
}

void PlayCommand::skipUpdate()
{
	finishedAction = true;
	currentVolume = finalVolume;
	wait = false;
	done = true;
}

void PlayCommand::update(float delta_t)
{
	if (valid && flagType == FLAG_FADEIN)
	{
		float offset = delta_t / time * finalVolume;
		currentVolume += offset;

		if (currentVolume >= finalVolume)
		{
			finishedAction = true;
			currentVolume = finalVolume;
		}
	}
	else
	{
		finishedAction = true;
	}
}
