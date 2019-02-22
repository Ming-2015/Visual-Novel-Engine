#include "ResumeCommand.h"

ResumeCommand::ResumeCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "resume")
	{
		LOGGER->Log("ResumeCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_RESUME;

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	// column 2 : object type character or background
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				// column 3 : flag

	// optional argument: time
	time = 1.5f;
	if (args.size() > COLUMN_ARG1)
	{
		try {
			time = std::stof(args[COLUMN_ARG1]); // column 8: time it should take to full finish the fade
		}
		catch (exception e)
		{
			LOGGER->Log("ResumeCommand", "Failed to convert time into float");
		}
	}

	finalVolume = 1.0f;
	if (args.size() > COLUMN_ARG2)
	{
		try {
			finalVolume = std::stof(args[COLUMN_ARG2]); // column 8: time it should take to full finish the fade
		}
		catch (exception e)
		{
			LOGGER->Log("ResumeCommand", "Failed to convert volume into float");
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
		LOGGER->Log("ResumeCommand", "Invalid Flag");
		valid = false;
		return;
	}

	// check object type validity
	if (objectTypeName == "bgm" || objectTypeName == "music" || objectTypeName == "background music" || objectTypeName == "b")
	{
		objectType = OBJECT_BGM;
	}
	else if (objectTypeName == "voice" || objectTypeName == "v")
	{
		objectType = OBJECT_VOICE;
	}
	else if (objectTypeName == "sfx" || objectTypeName == "sound effect" || objectTypeName == "" || objectTypeName == "sound" || objectTypeName == "s")
	{
		objectType = OBJECT_SFX;
	}
	else if (objectTypeName == "all" || objectTypeName == "a")
	{
		objectType = OBJECT_ALL;
	}
	else
	{
		valid = false;
		LOGGER->Log("ResumeCommand", "Invalid Object Type");
		return;
	}
}

ResumeCommand::~ResumeCommand()
{
}

ResumeCommand::ResumeCommand(ifstream & savefile)
	:ScriptCommand(savefile)
{
	try 
	{
		objectTypeName = UTILITY->readFromBinaryFile(savefile);
		flag = UTILITY->readFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *> (&time), sizeof(time));
		savefile.read(reinterpret_cast<char *> (&finalVolume), sizeof(finalVolume));
		savefile.read(reinterpret_cast<char *> (&currentVolume), sizeof(currentVolume));

		savefile.read(reinterpret_cast<char *> (&flagType), sizeof(flagType));
		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
		savefile.read(reinterpret_cast<char *> (&finishedAction), sizeof(finishedAction));
		savefile.read(reinterpret_cast<char *> (&played), sizeof(played));
	}
	catch (exception e)
	{
		LOGGER->Log("ResumeCommand", "Unable to read Resume Command from savedata");
		valid = false;
		throw;
	}
	played = false;
}

void ResumeCommand::serialize(ofstream & savefile) const
{
	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);

	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));
	savefile.write(reinterpret_cast<const char *> (&finalVolume), sizeof(finalVolume));
	savefile.write(reinterpret_cast<const char *> (&currentVolume), sizeof(currentVolume));

	savefile.write(reinterpret_cast<const char *> (&flagType), sizeof(flagType));
	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
	savefile.write(reinterpret_cast<const char *> (&finishedAction), sizeof(finishedAction));
	savefile.write(reinterpret_cast<const char *> (&played), sizeof(played));
}

void ResumeCommand::execute(ScriptLine * scriptLine)
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
					scriptLine->resumeBgm();
					break;
				}
				case OBJECT_SFX:
				{
					scriptLine->resumeSfx();
					break;
				}
				case OBJECT_VOICE:
				{
					scriptLine->resumeVoice();
					break;
				}
				case OBJECT_ALL:
				{
					scriptLine->resumeBgm();
					scriptLine->resumeSfx();
					scriptLine->resumeVoice();
					break;
				}
			}
		}

		if (flagType == FLAG_NONE)
		{
			switch (objectType)
			{
				case OBJECT_BGM:
				{
					scriptLine->setBgmVolume(finalVolume);
					break;
				}
				case OBJECT_SFX:
				{
					scriptLine->setSfxVolume(finalVolume);
					break;
				}
				case OBJECT_VOICE:
				{
					scriptLine->setVoiceVolume(finalVolume);
					break;
				}
				case OBJECT_ALL:
				{
					scriptLine->setBgmVolume(finalVolume);
					scriptLine->setSfxVolume(finalVolume);
					scriptLine->setVoiceVolume(finalVolume);
				}
			}

			done = true;
			wait = false;
		}
		else if (flagType == FLAG_FADEIN)
		{
			switch (objectType)
			{
				case OBJECT_BGM:
				{
					scriptLine->setBgmVolume(currentVolume);
					break;
				}
				case OBJECT_SFX:
				{
					scriptLine->setSfxVolume(currentVolume);
					break;
				}
				case OBJECT_VOICE:
				{
					scriptLine->setVoiceVolume(currentVolume);
					break;
				}
				case OBJECT_ALL:
				{
					scriptLine->setBgmVolume(currentVolume);
					scriptLine->setSfxVolume(currentVolume);
					scriptLine->setVoiceVolume(currentVolume);
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
		LOGGER->Log("ResumeCommand", "Attempting to execute invalid command");
		done = true;
	}
}

void ResumeCommand::skipUpdate()
{
	finishedAction = true;
	currentVolume = finalVolume;
	wait = false;
	done = true;
}

void ResumeCommand::update(float delta_t)
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

