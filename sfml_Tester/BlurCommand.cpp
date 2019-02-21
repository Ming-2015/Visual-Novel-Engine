#include "BlurCommand.h"

BlurCommand::BlurCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "blur")
	{
		LOGGER->Log("ShowCommand", "Invalid Command Input");
		valid = false;
		return;
	}

	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);

	objectName = UTILITY->toLower(args[COLUMN_ARG1]);
	srcFile = UTILITY->toLower(args[COLUMN_ARG2]);

	if (args.size() > COLUMN_ARG3)
	{
		try {
			blurRadius = stof(args[COLUMN_ARG3]);
		}
		catch (exception e)
		{
			LOGGER->Log("BlurCommand", "Failed to convert blurRadius into float");
		}
	}

	time = 1.5f;
	if (args.size() > COLUMN_ARG4)
	{
		try {
			time = std::stof(args[COLUMN_ARG4]);
		}
		catch (exception e)
		{
			LOGGER->Log("BlurCommand", "Failed to convert time into float");
		}
	}

	if (flag == "" || flag == "none" || time == 0)
	{
		wait = false;
		animationType = ANIMATION_NONE;
		time = 0;
		currentBlurR = blurRadius;
	}
	else if (flag == "wait" || flag == "w")
	{
		wait = true;
		animationType = ANIMATION_NONE;
		currentBlurR = blurRadius;
	}
	else if (flag == "gradual" || flag == "g")
	{
		wait = false;
		animationType = ANIMATION_GRADUAL;
		currentBlurR = 0;
	}
	else if (flag == "gradualwait" || flag == "gw")
	{
		wait = true;
		animationType = ANIMATION_GRADUAL;
		currentBlurR = 0;
	}
	else
	{
		LOGGER->Log("BlurCommand", "Invalid Flag");
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
		LOGGER->Log("BlurCommand", "Invalid Object Type");
		return;
	}

}

BlurCommand::~BlurCommand()
{

}

void BlurCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->tickCharacterShader(objectName, true);
			scriptLine->setCharacterShader(objectName, srcFile);
			scriptLine->setCharacterBlurRadius(objectName, currentBlurR);

			std::string msg = "Blur radius: " + to_string(currentBlurR);
			LOGGER->Log("BlurCommand", msg);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->tickBackgroundShader(objectName, true);
			scriptLine->setBackgroundShader(objectName, srcFile);
			scriptLine->setBackgroundBlurRadius(objectName, currentBlurR);
		}
		else if (objectType == OBJECT_ALL)
		{
			scriptLine->tickBackgroundShader(objectName, true);
			scriptLine->tickCharacterShader(objectName, true);
			scriptLine->setCharacterShader(objectName, srcFile);
			scriptLine->setBackgroundShader(objectName, srcFile);
			scriptLine->setCharacterBlurRadius(objectName, currentBlurR);
			scriptLine->setBackgroundBlurRadius(objectName, currentBlurR);
		}
	}
	else
	{
		done = true;
	}
}

void BlurCommand::skipUpdate()
{
	currentBlurR = blurRadius;
	wait = false;
	done = true;
}

void BlurCommand::update(float delta_t)
{
	if (valid && time > 0)
	{
		if (animationType == ANIMATION_GRADUAL && currentBlurR != blurRadius)
		{
			float blurOffset = delta_t / time * blurRadius;

			currentBlurR += blurOffset;
			if (currentBlurR >= blurRadius)
			{
				currentBlurR = blurRadius;
				wait = false;
				done = true;
			}
		}
	}
}