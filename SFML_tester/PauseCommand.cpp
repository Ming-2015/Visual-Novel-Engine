#include "PauseCommand.h"

PauseCommand::PauseCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "pause")
	{
		LOGGER->Log("PauseCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	// column 2 : object type character or background
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				// column 3 : flag

	objectName = args[COLUMN_ARG1];		// column 4 : object name
	objectSubname = args[COLUMN_ARG2];	// column 5 : object sub name

	finalVolume = 0.0f;
	currentVolume = 1.0f;
	prevVolume = 1.0f;

	// optional argument: time
	time = 1.5f;
	if (args.size() > COLUMN_ARG1)
	{
		try {
			time = std::stof(args[COLUMN_ARG1]); // column 8: time it should take to full finish the fade
		}
		catch (exception e)
		{
			LOGGER->Log("PauseCommand", "Failed to convert time into float");
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
		LOGGER->Log("PauseCommand", "Invalid Flag");
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
		LOGGER->Log("PauseCommand", "Invalid Object Type");
		return;
	}
}

PauseCommand::~PauseCommand()
{
}

void PauseCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (flagType == FLAG_NONE)
		{
			switch (objectType)
			{
			case OBJECT_BGM:
			{
				scriptLine->pauseBgm();
				break;
			}
			case OBJECT_SFX:
			{
				scriptLine->pauseSfx();
				break;
			}
			case OBJECT_VOICE:
			{
				scriptLine->pauseVoice();
				break;
			}
			case OBJECT_ALL:
			{
				scriptLine->pauseVoice();
				scriptLine->pauseSfx();
				scriptLine->pauseBgm();
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
					scriptLine->setBgmVolume(1.f / prevVolume, true);
					scriptLine->setBgmVolume(currentVolume, true);
					break;
				}
				case OBJECT_SFX:
				{
					scriptLine->setSfxVolume(1.f / prevVolume, true);
					scriptLine->setSfxVolume(currentVolume, true);
					break;
				}
				case OBJECT_VOICE:
				{
					scriptLine->setVoiceVolume(1.f / prevVolume, true);
					scriptLine->setVoiceVolume(currentVolume, true);
					break;
				}
				case OBJECT_ALL:
				{
					scriptLine->setVoiceVolume(1.f / prevVolume, true);
					scriptLine->setVoiceVolume(currentVolume, true);
					scriptLine->setSfxVolume(1.f / prevVolume, true);
					scriptLine->setSfxVolume(currentVolume, true);
					scriptLine->setBgmVolume(1.f / prevVolume, true);
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
						scriptLine->pauseBgm();
						break;
					}
					case OBJECT_SFX:
					{
						scriptLine->pauseSfx();
						break;
					}
					case OBJECT_VOICE:
					{
						scriptLine->pauseVoice();
						break;
					}
					case OBJECT_ALL:
					{
						scriptLine->pauseVoice();
						scriptLine->pauseSfx();
						scriptLine->pauseBgm();
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
		LOGGER->Log("PauseCommand", "Attempting to execute invalid command");
		done = true;
	}
}

void PauseCommand::skipUpdate()
{
	finishedAction = true;
	prevVolume = currentVolume;
	currentVolume = finalVolume;
	wait = false;
	done = true;
}

void PauseCommand::update(float delta_t)
{
	if (valid && flagType == FLAG_FADEOUT)
	{
		prevVolume = currentVolume;

		float offset = delta_t / time * (1.0f);
		currentVolume -= offset;

		if (currentVolume <= finalVolume)
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
