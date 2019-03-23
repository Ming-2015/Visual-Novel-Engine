#include "SavedataReader.h"

bool SavedataReader::loadFromFile(std::string path, bool readScriptManager)
{
	filepath = path;
	try
	{
		if (!readScriptManager)
		{
			SAVEDATAUTILITY->readSave(filepath, saveImage, title, date);
		}
		else
		{
			SAVEDATAUTILITY->readSave(filepath, saveImage, title, date, scriptManager);
		}

		loaded = true;
		return true;
	}
	catch (exception e)
	{
		std::string err = "Unable to read savefile: " + path;
		LOGGER->Log("SavedataReader", err);

		loaded = false;
		return false;
	}
}

bool SavedataReader::isLoaded() const
{
	return loaded;
}

void SavedataReader::cleanScriptManager()
{
	if (scriptManager)
	{
		delete scriptManager;
		scriptManager = nullptr;
	}
}

const sf::Image & SavedataReader::getSaveImage() const
{
	return saveImage;
}

std::string SavedataReader::getTitleStr() const
{
	return title;
}

std::string SavedataReader::getDateStr() const
{
	return date;
}

std::string SavedataReader::getFilePath() const
{
	return filepath;
}

ScriptManager * SavedataReader::getScriptManager() const
{
	return scriptManager;
}


