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
	commandType = COMMAND_SET;

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
			//LOGGER->Log("ShowCommand", "Failed to convert x and y values into floats");
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
			//LOGGER->Log("ShowCommand", "Failed to convert time into float");
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

SetCommand::SetCommand(ifstream & savefile)
	:ScriptCommand(savefile)
{
	try 
	{
		objectTypeName = UTILITY->readFromBinaryFile(savefile);
		flag = UTILITY->readFromBinaryFile(savefile);
		objectName = UTILITY->readFromBinaryFile(savefile);
		objectSubname = UTILITY->readFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *> (&x1), sizeof(x1));
		savefile.read(reinterpret_cast<char *> (&y1), sizeof(y1));
		savefile.read(reinterpret_cast<char *> (&time), sizeof(time));

		savefile.read(reinterpret_cast<char *> (&animationType), sizeof(animationType));
		savefile.read(reinterpret_cast<char *> (&alpha), sizeof(alpha));

		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
		savefile.read(reinterpret_cast<char *> (&initiailized), sizeof(initiailized));
	}
	catch (exception e)
	{
		LOGGER->Log("SetCommand", "Unable to load Set command from savdata");
		valid = false;
		throw;
	}
}

void SetCommand::serialize(ofstream & savefile) const
{

	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);
	UTILITY->writeToBinaryFile(savefile, objectName);
	UTILITY->writeToBinaryFile(savefile, objectSubname);

	savefile.write(reinterpret_cast<const char *> (&x1), sizeof(x1));
	savefile.write(reinterpret_cast<const char *> (&y1), sizeof(y1));
	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));

	savefile.write(reinterpret_cast<const char *> (&animationType), sizeof(animationType));
	savefile.write(reinterpret_cast<const char *> (&alpha), sizeof(alpha));

	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
	savefile.write(reinterpret_cast<const char *> (&initiailized), sizeof(initiailized));

}

void SetCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			if (!initiailized) {
				scriptLine->removeAllCharacters();
				scriptLine->setCharacter(objectName, objectSubname, x1, y1);
				scriptLine->setCharacterZoom(objectName, objectSubname, 1, 1);
				scriptLine->setCharacterRotation(objectName, objectSubname, true, 0);
				initiailized = true;
			}

			scriptLine->setCharacterAlpha(objectName, alpha);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			if (!initiailized) {
				scriptLine->removeAllBackgrounds();
				scriptLine->setBackground(objectName, objectSubname, x1, y1);
				scriptLine->setBackgroundZoom(objectName, objectSubname, 1, 1);
				scriptLine->setBackgroundRotation(objectName, objectSubname, true, 0);
				initiailized = true;
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