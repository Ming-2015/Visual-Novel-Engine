#include "ClearCommand.h"

ClearCommand::ClearCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "clear")
	{
		LOGGER->Log("ClearCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_CLEAR;

	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);

	time = .5f;
	if (args.size() > COLUMN_ARG1)
	{
		try {
			time = stof(args[COLUMN_ARG1]);
		}
		catch (exception e)
		{
			LOGGER->Log("ClearCommand", "Failed to convert time into float");
		}
	}

	if (flag == "" || flag == "none" || time == 0)
	{
		wait = false;
		animationType = ANIMATION_NONE;
		time = 0;
		alpha = 0.0f;
	}
	else if (flag == "fadeout")
	{
		wait = false;
		animationType = ANIMATION_FADEOUT;
		alpha = 255;
	}
	else if (flag == "fadeoutwait")
	{
		wait = true;
		animationType = ANIMATION_FADEOUT;
		alpha = 255;
	}
	else
	{
		LOGGER->Log("ClearCommand", "Invalid Flag");
		valid = false;
		return;
	}

	if (objectTypeName == "character" || objectTypeName == "c" || objectTypeName == "char")
	{
		objectType = OBJECT_CHARACTER;
	}
	else if (objectTypeName == "background" || objectTypeName == "b" || objectTypeName == "bg")
	{
		objectType = OBJECT_BACKGROUND;
	}
	else
	{
		valid = false;
		LOGGER->Log("ClearCommand", "Invalid Object Type");
		return;
	}
}

ClearCommand::~ClearCommand()
{

}

ClearCommand::ClearCommand(ifstream & savefile)
	:ScriptCommand(savefile)
{
	try {
		objectTypeName = UTILITY->readFromBinaryFile(savefile);
		flag = UTILITY->readFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *> (&time), sizeof(time));

		savefile.read(reinterpret_cast<char *> (&animationType), sizeof(animationType));
		savefile.read(reinterpret_cast<char *> (&alpha), sizeof(alpha));

		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
	}
	catch (exception e)
	{
		LOGGER->Log("ClearCommand", "Unable to read clear command");
		valid = false;
		return;
	}
}

void ClearCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);

	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));

	savefile.write(reinterpret_cast<const char *> (&animationType), sizeof(animationType));
	savefile.write(reinterpret_cast<const char *> (&alpha), sizeof(alpha));

	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
}

void ClearCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->setAllCharacterAlpha(alpha);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setAllBackgroundAlpha(alpha);
		}

		if ((animationType == ANIMATION_NONE || alpha <= 0.0f) && objectType == OBJECT_CHARACTER)
		{
			done = true;
			scriptLine->removeAllCharacters();
		}
		else if ((animationType == ANIMATION_NONE || alpha <= 0.0f) && objectType == OBJECT_BACKGROUND)
		{
			done = true;
			scriptLine->removeAllBackgrounds();
		}
	}
	else
	{
		done = true;
	}
}

void ClearCommand::skipUpdate()
{
	alpha = 0.0f;
	wait = false;
	done = true;
}

void ClearCommand::update(float delta_t)
{
	if (valid && time > 0)
	{
		if (animationType == ANIMATION_FADEOUT && alpha > 0.0f)
		{
			float alpha_offset = delta_t / time * 255.f;

			alpha -= alpha_offset;
			if (alpha <= 0.0f)
			{
				alpha = 0.0f;
				wait = false;
			}
		}
	}
}