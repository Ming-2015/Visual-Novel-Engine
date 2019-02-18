#include "FlashCommand.h"

FlashCommand::FlashCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "flash")
	{
		LOGGER->Log("FlashCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);
	objectName = UTILITY->toLower(args[COLUMN_ARG1]);
	numFlash = 7;
	if (args.size() > COLUMN_ARG2)
	{
		try {
			numFlash = stoi(args[COLUMN_ARG2]);
		}
		catch (exception e)
		{
			LOGGER->Log("FlashCommand", "Failed to convert numFlash into int");
		}
	}
	time = 1.5f;
	if (args.size() > COLUMN_ARG3)
	{
		try {
			time = stof(args[COLUMN_ARG3]);
		}
		catch (exception e)
		{
			cout << endl << "FAILED TO CONVERT THE TIME HERE";
			LOGGER->Log("FlashCommand", "Failed to convert time into float");
		}
	}
	keep = true;
	if (args.size() > COLUMN_ARG4)
	{
		try {
			keep = UTILITY->str2bool(args[COLUMN_ARG4]);
		}
		catch (exception e)
		{
			LOGGER->Log("FlashCommand", "Failed to convert keep into bool");
		}
	}

	if (flag == "" || flag == "none")
	{
		wait = false;
		animationType = ANIMATION_FLASH;
		time = 0;
	}
	else if (flag == "wait" || flag == "w")
	{
		wait = true;
		animationType = ANIMATION_FLASH;
		time = 0;
	}
	else if (flag == "fadeout" || flag == "f")
	{
		wait = false;
		animationType = ANIMATION_FLASHFADEOUT;
	}
	else if (flag == "fadeoutwait" || flag == "fw" || flag == "fow")
	{
		wait = true;
		animationType = ANIMATION_FLASHFADEOUT;
	}
	else
	{
		LOGGER->Log("FadeCommand", "Invalid Flag");
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
	else if (objectTypeName == "all" || "a")
	{
		objectType = OBJECT_ALL;
	}
	else
	{
		valid = false;
		LOGGER->Log("FlashCommand", "Invalid Object Type");
		return;
	}

	flashCount = 0;
	showBlack = 0;
}

FlashCommand::~FlashCommand()
{

}

void FlashCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->setAllCharacterAlpha(alpha);
			//NEED TO MAKE IT ONLY ONE CHAR
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setAllBackgroundAlpha(alpha);
		}
		else if (objectType == OBJECT_ALL)
		{
			scriptLine->setAllBackgroundAlpha(alpha);
			scriptLine->setAllCharacterAlpha(alpha);
		}

		if (keep == false && doneFade == true)
		{
			if (objectType == OBJECT_CHARACTER)
			{
				done = true;
				scriptLine->removeAllCharacters();
			}
			else if (objectType == OBJECT_BACKGROUND)
			{
				done = true;
				scriptLine->removeAllBackgrounds();
			}
			else if (objectType == OBJECT_ALL)
			{
				done = true;
				scriptLine->removeAllBackgrounds();
				scriptLine->removeAllCharacters();
			}
		}
	}
	else
	{
		done = true;
	}
}

void FlashCommand::skipUpdate()
{
	if (keep == true)
	{
		alpha = 255.0f;
	}
	else if (keep == false)
	{
		alpha = 0;
	}
	wait = false;
	done = true;
}

void FlashCommand::update(float delta_t)
{
	if (valid)
	{
		if (doneFlash == false)
		{
			if (clock.getElapsedTime().asMilliseconds() > 60.0f)
			{
				if (alpha == 0.0 && flashCount < numFlash)
				{
					alpha = 255.0;
					flashCount++;
					clock.restart();
				}
				else if (alpha == 255.0 && flashCount < numFlash)
				{
					alpha = 0.0;
					flashCount++;
					clock.restart();
				}
				else
				{
					doneFlash = true;
					showBlack = 0;
					clock.restart();
				}
			}	
		}
		if (doneFlash == true &&  doneFade == false && animationType == ANIMATION_FLASHFADEOUT)
		{
			if (alpha > 0.0)
			{
				float alphaOffset = delta_t / time * 255.f;
				alpha -= alphaOffset;
				if (alpha <= 0.0f)
				{
					alpha = 0.0f;
				}
			}
			if (alpha == 0.0f && showBlack < 45)
			{
				cout << "running show black" << endl;
				showBlack++;
			}
			else if (alpha == 0.0f && showBlack >= 45)
			{
				cout << endl << "I TICKED THE WRONG FLAGS" << endl;
				doneFade = true;
				done = true;
			}
		}
	}
}