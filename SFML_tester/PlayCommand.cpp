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
			string msg = "Time = " + to_string(time);
			LOGGER->Log("PlayCommand", msg);
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
	else if (flag == "fadeinwait")
	{
		wait = true;
		flagType = FLAG_FADEIN;
	}
	else if (flag == "fadein")
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

		string msg = "Increasing volume by " + to_string(offset) + ", currentVolume = " + to_string(currentVolume);
		LOGGER->Log("PlayCommand", msg);

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
