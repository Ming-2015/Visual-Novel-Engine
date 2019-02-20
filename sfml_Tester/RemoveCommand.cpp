#include "RemoveCommand.h"

RemoveCommand::RemoveCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "remove")
	{
		LOGGER->Log("RemoveCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);

	objectName = UTILITY->toLower(args[COLUMN_ARG1]);

	time = .5f;
	if (args.size() > COLUMN_ARG2)
	{
		try {
			time = stof(args[COLUMN_ARG2]);
		}
		catch (exception e)
		{
			LOGGER->Log("RemoveCommand", "Failed to convert time into float");
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
		LOGGER->Log("RemoveCommand", "Invalid Flag");
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
	else if (objectTypeName == "flag" || objectTypeName == "f")
	{
		objectType = OBJECT_FLAG;
		if (animationType != ANIMATION_NONE)
		{
			valid = false;
			LOGGER->Log("RemoveCommand", "Invalid flag for Remove Flag");
			return;
		}
	}
	else
	{
		valid = false;
		LOGGER->Log("RemoveCommand", "Invalid Object Type");
		return;
	}
}

RemoveCommand::~RemoveCommand()
{

}

void RemoveCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->setCharacterAlpha(objectName, alpha);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setBackgroundAlpha(objectName, alpha);
		}
		else if (objectType == OBJECT_FLAG)
		{
			auto it = GLOBAL->userFlags.find(objectName);
			if (it != GLOBAL->userFlags.end())
			{
				GLOBAL->userFlags.erase(it);
			}
			done = true;
		}

		if ((animationType == ANIMATION_NONE || alpha <= 0.0f) && objectType == OBJECT_CHARACTER)
		{
			done = true;
			scriptLine->removeCharacter(objectName);
		}
		else if ((animationType == ANIMATION_NONE || alpha <= 0.0f) && objectType == OBJECT_BACKGROUND)
		{
			done = true;
			scriptLine->removeBackground(objectName);
		}
	}
	else
	{
		done = true;
	}
}

void RemoveCommand::skipUpdate()
{
	alpha = 0.0f;
	wait = false;
	done = true;
}

void RemoveCommand::update(float delta_t)
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
				wait= false;
			}
		}
	}
}
