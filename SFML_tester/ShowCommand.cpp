#include "ShowCommand.h"

ShowCommand::ShowCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "show")
	{
		LOGGER->Log("ShowCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_SHOW;

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	// column 2 : object type character or background
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				// column 3 : flag

	objectName = UTILITY->toLower(args[COLUMN_ARG1]);		// column 4 : object name
	objectSubname = UTILITY->toLower(args[COLUMN_ARG2]);	// column 5 : object sub name

	// optional arguments: x and y coordinates
	x1 = 0; 
	y1 = 0;
	if (args.size() > COLUMN_ARG4)
	{
		try {
			x1 = std::stof(args[COLUMN_ARG3]);	// column 6 : x position to display
			y1 = std::stof(args[COLUMN_ARG4]);	// column 7 : y position to display
		}
		catch (exception e)
		{
			LOGGER->Log("ShowCommand", "Failed to convert x and y values into floats");
		}
	}

	// optional argument: time
	time = .5f;
	if (args.size() > COLUMN_ARG5)
	{
		try {
			time = std::stof(args[COLUMN_ARG5]); // column 8: time it should take to full finish the fade
		}
		catch (exception e)
		{
			LOGGER->Log("ShowCommand", "Failed to convert time into float");
		}
	}

	// check the flag validity
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

	// check object type validity
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

ShowCommand::~ShowCommand()
{

}

ShowCommand::ShowCommand(ifstream & savefile)
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
		savefile.read(reinterpret_cast<char *> (&initialized), sizeof(initialized));

		savefile.read(reinterpret_cast<char *> (&animationType), sizeof(animationType));
		savefile.read(reinterpret_cast<char *> (&alpha), sizeof(alpha));
		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));
	}
	catch (exception e)
	{
		LOGGER->Log("ShowCommand", "Unable to read Show Commannd from savedata");
		valid = false;
		throw;
	}
}

void ShowCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);
	UTILITY->writeToBinaryFile(savefile, objectName);
	UTILITY->writeToBinaryFile(savefile, objectSubname);

	savefile.write(reinterpret_cast<const char *> (&x1), sizeof(x1));
	savefile.write(reinterpret_cast<const char *> (&y1), sizeof(y1));
	savefile.write(reinterpret_cast<const char *> (&time), sizeof(time));
	savefile.write(reinterpret_cast<const char *> (&initialized), sizeof(initialized));

	savefile.write(reinterpret_cast<const char *> (&animationType), sizeof(animationType));
	savefile.write(reinterpret_cast<const char *> (&alpha), sizeof(alpha));
	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));
}

void ShowCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_CHARACTER)
		{
			if (!initialized)
			{
				scriptLine->setCharacter(objectName, objectSubname, x1, y1);
				scriptLine->setCharacterZoom(objectName, objectSubname, 1, 1);
				scriptLine->setCharacterRotation(objectName, objectSubname, true, 0);
				initialized = true;
				
			}
			scriptLine->setCharacterAlpha(objectName, alpha);
		}
		else if (objectType == OBJECT_BACKGROUND)
		{
			if (!initialized)
			{
				scriptLine->setBackground(objectName, objectSubname, x1, y1);
				scriptLine->setBackgroundZoom(objectName, objectSubname, 1, 1);
				scriptLine->setBackgroundRotation(objectName, objectSubname, true, 0);
				initialized = true;
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

void ShowCommand::skipUpdate()
{
	alpha = 255.f;
	wait = false;
	done = true;
}

void ShowCommand::update(float delta_t)
{
	if (valid && time > 0)
	{
		if (animationType == ANIMATION_FADEIN && alpha < 255.f)
		{
			float alpha_offset = delta_t / time * 255.f;

			//string msg = "Alpha Offset = " + to_string(alpha_offset) + "\tdelta_t = " + to_string(delta_t);
			//LOGGER->Log("ShowCommand", msg);

			alpha += alpha_offset;
			if (alpha >= 255.f)
			{
				alpha = 255.f;
				wait = false;
			}
		}
	}
}

