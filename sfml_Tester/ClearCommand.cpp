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

	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);

	time = 1.5f;
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