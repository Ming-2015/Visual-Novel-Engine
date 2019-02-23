#include "BlurCommand.h"

BlurCommand::BlurCommand(vector<string> args)
	: ScriptCommand(args)
{
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "blur")
	{
		LOGGER->Log("BlurCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = ScriptCommand::COMMAND_BLUR;

	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);
	flag = UTILITY->toLower(args[COLUMN_FLAG]);

	// object name in arg1
	objectName = UTILITY->toLower(args[COLUMN_ARG1]);

	// blur radius in arg2
	blurRadius = 0;
	if (args.size() > COLUMN_ARG2)
	{
		try {
			blurRadius = stof(args[COLUMN_ARG2]);
		}
		catch (exception e)
		{
			LOGGER->Log("BlurCommand", "Failed to convert blurRadius into float");
		}
	}

	time = 1.5f;
	if (args.size() > COLUMN_ARG3)
	{
		try {
			time = std::stof(args[COLUMN_ARG3]);
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
	else if (flag == "gradualrelative" || flag == "gr")
	{
		wait = false;
		relative = true;
		animationType = ANIMATION_NONE;
	}
	else if (flag == "gradualwaitrelative" || flag == "gwr")
	{
		wait = true;
		animationType = ANIMATION_GRADUAL;
		relative = true;
		//set currentBlurR in execute
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

BlurCommand::BlurCommand(ifstream & savefile)
	:ScriptCommand(savefile)
{
	try {
		objectTypeName = UTILITY->readFromBinaryFile(savefile);
		flag = UTILITY->readFromBinaryFile(savefile);
		objectName = UTILITY->readFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *> (&time), sizeof(time));
		savefile.read(reinterpret_cast<char *> (&initialized), sizeof(initialized));

		savefile.read(reinterpret_cast<char *> (&animationType), sizeof(animationType));
		savefile.read(reinterpret_cast<char *> (&blurRadius), sizeof(blurRadius));
		savefile.read(reinterpret_cast<char *> (&currentBlurR), sizeof(currentBlurR));

		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));

		savefile.read(reinterpret_cast<char *> (&relative), sizeof(relative));
		savefile.read(reinterpret_cast<char *> (&firstLoopRel), sizeof(firstLoopRel));
		savefile.read(reinterpret_cast<char *> (&lastLoopRel), sizeof(lastLoopRel));
		savefile.read(reinterpret_cast<char *> (&blurDiff), sizeof(blurDiff));
	}
	catch (exception e)
	{
		LOGGER->Log("BlurCommand", "Unable to read blur command");
		valid = false;
		return;
	}
}

void BlurCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);
	UTILITY->writeToBinaryFile(savefile, objectName);

	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));
	savefile.write(reinterpret_cast<const char *> (&initialized), sizeof(initialized));

	savefile.write(reinterpret_cast<const char *> (&animationType), sizeof(animationType));
	savefile.write(reinterpret_cast<const char *> (&blurRadius), sizeof(blurRadius));
	savefile.write(reinterpret_cast<const char *> (&currentBlurR), sizeof(currentBlurR));

	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));

	savefile.write(reinterpret_cast<const char *> (&relative), sizeof(relative));
	savefile.write(reinterpret_cast<const char *> (&firstLoopRel), sizeof(firstLoopRel));
	savefile.write(reinterpret_cast<const char *> (&lastLoopRel), sizeof(lastLoopRel));
	savefile.write(reinterpret_cast<const char *> (&blurDiff), sizeof(blurDiff));
}

BlurCommand::~BlurCommand()
{

}

void BlurCommand::execute(ScriptLine * scriptLine)
{
	if (valid && relative == false)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->tickCharacterShader(objectName, true);
			scriptLine->setCharacterShader(objectName, GLOBAL->blurFragShaderPath);
			scriptLine->setCharacterBlurRadius(objectName, currentBlurR);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->tickBackgroundShader(objectName, true);
			scriptLine->setBackgroundShader(objectName, GLOBAL->blurFragShaderPath);
			scriptLine->setBackgroundBlurRadius(objectName, currentBlurR);
		}
		else if (objectType == OBJECT_ALL)
		{
			scriptLine->tickBackgroundShader(objectName, true);
			scriptLine->tickCharacterShader(objectName, true);
			scriptLine->setCharacterShader(objectName, GLOBAL->blurFragShaderPath);
			scriptLine->setBackgroundShader(objectName, GLOBAL->blurFragShaderPath);
			scriptLine->setCharacterBlurRadius(objectName, currentBlurR);
			scriptLine->setBackgroundBlurRadius(objectName, currentBlurR);
		}
	}
	else if (valid && relative == true && firstLoopRel == true)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			currentBlurR = scriptLine->getCharacterBeginBlurR(objectName);
			blurDiff = blurRadius - currentBlurR;
			firstLoopRel = false;
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			currentBlurR = scriptLine->getCharacterBeginBlurR(objectName);
			blurDiff = blurRadius - currentBlurR;
			firstLoopRel = false;
		}
		else if (objectType == OBJECT_ALL)
		{
			//If bgBeginBlurR =/= charBeginBlurR, relative blur will not work well
			currentBlurR = scriptLine->getCharacterBeginBlurR(objectName);
			blurDiff = blurRadius - currentBlurR;
			firstLoopRel = false;
		}
	}
	else if (valid && relative == true && firstLoopRel == false)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			scriptLine->setCharacterBlurRadius(objectName, currentBlurR);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			scriptLine->setBackgroundBlurRadius(objectName, currentBlurR);
		}
		else if (objectType == OBJECT_ALL)
		{
			scriptLine->setCharacterBlurRadius(objectName, currentBlurR);
			scriptLine->setBackgroundBlurRadius(objectName, currentBlurR);

		}
		if (animationType == ANIMATION_NONE)
		{
			done = true;
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
	if (valid && relative == false)
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
	else if (valid && relative == true)
	{
		if (firstLoopRel == true)
		{

		}
		else if (animationType == ANIMATION_GRADUAL && currentBlurR != blurRadius)
		{
			float blurOffset = delta_t / time * blurDiff;

			currentBlurR += blurOffset;
			if (blurDiff == 0)
			{
				lastLoopRel = true;
				wait = false;
				done = true;
			}
			else if (blurDiff > 0)
			{
				if (currentBlurR >= blurRadius)
				{
					wait = false;
					done = true;
				}
			}
			else if (blurDiff < 0)
			{
				if (currentBlurR <= blurRadius)
				{
					currentBlurR = blurRadius;
					wait = false;
					done = true;
				}
			}
			
			if (currentBlurR == blurRadius)
			{
				wait = false;
				done = true;
			}
		}
	}
	else
	{
		wait = false;
		currentBlurR = blurRadius;
		done = true;
	}
}