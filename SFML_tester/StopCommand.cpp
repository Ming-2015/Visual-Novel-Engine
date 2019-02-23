#include "StopCommand.h"


StopCommand::StopCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "stop")
	{
		LOGGER->Log("StopCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_STOP;

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	// column 2 : object type character or background
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				// column 3 : flag

	objectName = args[COLUMN_ARG1];		// column 4 : object name
	objectSubname = args[COLUMN_ARG2];	// column 5 : object sub name

	finalVolume = 0.0f;
	currentVolume = 1.0f;
	timeElapsed = 0.0f;

	// optional argument: time
	time = 1.5f;
	if (args.size() > COLUMN_ARG1)
	{
		try {
			time = std::stof(args[COLUMN_ARG1]); // column 8: time it should take to full finish the fade
		}
		catch (exception e)
		{
			LOGGER->Log("StopCommand", "Failed to convert time into float");
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
	else if (flag == "fadeoutwait" || flag == "fw")
	{
		wait = true;
		flagType = FLAG_FADEOUT;
	}
	else if (flag == "fadeout" || flag == "f")
	{
		wait = false;
		flagType = FLAG_FADEOUT;
	}
	else
	{
		LOGGER->Log("StopCommand", "Invalid Flag");
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
		LOGGER->Log("StopCommand", "Invalid Object Type");
		return;
	}
}

StopCommand::~StopCommand()
{
}

StopCommand::StopCommand(ifstream & savefile)
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
		savefile.read(reinterpret_cast<char *> (&timeElapsed), sizeof(timeElapsed));

		savefile.read(reinterpret_cast<char *> (&flagType), sizeof(flagType));
		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
		savefile.read(reinterpret_cast<char *> (&finishedAction), sizeof(finishedAction));
	}
	catch (exception e)
	{
		LOGGER->Log("StopCommand", "Unable to read Stop Command from savedata");
		valid = false;
		throw;
	}
}

void StopCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);
	UTILITY->writeToBinaryFile(savefile, objectName);
	UTILITY->writeToBinaryFile(savefile, objectSubname);

	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));
	savefile.write(reinterpret_cast<const char *> (&finalVolume), sizeof(finalVolume));
	savefile.write(reinterpret_cast<const char *> (&currentVolume), sizeof(currentVolume));
	savefile.write(reinterpret_cast<const char *> (&timeElapsed), sizeof(timeElapsed));

	savefile.write(reinterpret_cast<const char *> (&flagType), sizeof(flagType));
	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
	savefile.write(reinterpret_cast<const char *> (&finishedAction), sizeof(finishedAction));
}

void StopCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (flagType == FLAG_NONE)
		{
			switch (objectType)
			{
				case OBJECT_BGM:
				{
					scriptLine->stopBgm();
					break;
				}
				case OBJECT_SFX:
				{
					scriptLine->stopSfx();
					break;
				}
				case OBJECT_VOICE:
				{
					scriptLine->stopVoice();
					break;
				}
				case OBJECT_ALL:
				{
					scriptLine->stopVoice();
					scriptLine->stopSfx();
					scriptLine->stopBgm();
					break;
				}
			}
			done = true;
			wait = false;
		}
		else if (flagType == FLAG_FADEOUT)
		{
			switch (objectType)
			{
				case OBJECT_BGM:
				{
					scriptLine->setBgmVolume(currentVolume, true);
					break;
				}
				case OBJECT_SFX:
				{
					scriptLine->setSfxVolume(currentVolume, true);
					break;
				}
				case OBJECT_VOICE:
				{
					scriptLine->setVoiceVolume(currentVolume, true);
					break;
				}
				case OBJECT_ALL:
				{
					scriptLine->setVoiceVolume(currentVolume, true);
					scriptLine->setSfxVolume(currentVolume, true);
					scriptLine->setBgmVolume(currentVolume, true);
					break;
				}
			}

			if (finishedAction)
			{
				switch (objectType)
				{
					case OBJECT_BGM:
					{
						scriptLine->stopBgm();
						break;
					}
					case OBJECT_SFX:
					{
						scriptLine->stopSfx();
						break;
					}
					case OBJECT_VOICE:
					{
						scriptLine->stopVoice();
						break;
					}
					case OBJECT_ALL:
					{
						scriptLine->stopVoice();
						scriptLine->stopSfx();
						scriptLine->stopBgm();
						break;
					}
				}
				done = true;
				wait = false;
			}
		}
	}
	else
	{
		LOGGER->Log("StopCommand", "Attempting to execute invalid command");
		done = true;
	}
}

void StopCommand::skipUpdate()
{
	finishedAction = true;
	timeElapsed = time;
	currentVolume = finalVolume;
	wait = false;
	done = true;
}

void StopCommand::update(float delta_t)
{
	if (valid && flagType == FLAG_FADEOUT)
	{
		timeElapsed += delta_t;
		float reduction = (timeElapsed*timeElapsed) / (time*time);
		reduction = reduction * reduction;
		currentVolume = 1.0f - reduction;

		if (currentVolume <= finalVolume || timeElapsed >= time)
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
