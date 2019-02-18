#include "SetCommand.h"

SetCommand::SetCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "show")
	{
		LOGGER->Log("ShowCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);

	objectName = UTILITY->toLower(args[COLUMN_ARG1]);
	objectSubname = UTILITY->toLower(args[COLUMN_ARG2]);	

	x1 = 0;
	y1 = 0;
	if (args.size() > COLUMN_ARG4)
	{
		try {
			x1 = std::stof(args[COLUMN_ARG3]);	
			y1 = std::stof(args[COLUMN_ARG4]);	
		}
		catch (exception e)
		{
			LOGGER->Log("ShowCommand", "Failed to convert x and y values into floats");
		}
	}

	time = .5f;
	if (args.size() > COLUMN_ARG5)
	{
		try {
			time = std::stof(args[COLUMN_ARG5]);
		}
		catch (exception e)
		{
			LOGGER->Log("ShowCommand", "Failed to convert time into float");
		}
	}

	if (flag == "" || flag == "none" || time == 0)
	{
		wait = false;
		animationType = ANIMATION_NONE;
		time = 0;
		alpha = 255.f;
	}
	else if (flag == "fadeinwait")
	{
		wait = true;
		animationType = ANIMATION_FADEIN;
		alpha = 0;
	}
	else if (flag == "fadein")
	{
		wait = false;
		animationType = ANIMATION_FADEIN;
		alpha = 0;
	}
	else
	{
		LOGGER->Log("ShowCommand", "Invalid Flag");
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
		LOGGER->Log("ShowCommand", "Invalid Object Type");
		return;
	}
}

SetCommand::~SetCommand()
{

}

void SetCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			if (initiailized) {
				scriptLine->removeAllCharacters();
				scriptLine->setCharacter(objectName, objectSubname, x1, y1);
				initiailized = false;
			}

			scriptLine->setCharacterAlpha(objectName, alpha);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			if (initiailized) {
				scriptLine->removeAllBackgrounds();
				scriptLine->setBackground(objectName, objectSubname, x1, y1);
				initiailized = false;
			}

			scriptLine->setBackgroundAlpha(objectName, alpha);
		}

		if (animationType == ANIMATION_NONE || alpha >= 255.f - .1f)
		{
			done = true;
		}
	}
	else
	{
		done = true;
	}
}

void SetCommand::skipUpdate()
{
	alpha = 255.f;
	wait = false;
	done = true;
}

void SetCommand::update(float delta_t)
{
	if (valid && time > 0)
	{
		if (animationType == ANIMATION_FADEIN && alpha < 255.f)
		{
			float alpha_offset = delta_t / time * 255.f;

			alpha += alpha_offset;
			if (alpha >= 255.f)
			{
				alpha = 255.f;
				wait = false;
			}
		}
	}
	else
	{
		wait = false;
	}
}