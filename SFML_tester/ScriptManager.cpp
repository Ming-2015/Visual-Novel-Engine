#include "ScriptManager.h"

ScriptManager::ScriptManager(std::string filename, unsigned int initLineId) :
	filename(filename),
	currentLineId(initLineId)
{
	//init();
}

ScriptManager::~ScriptManager()
{
	if (currentScriptLine != nullptr)
		delete currentScriptLine;
	if (file.is_open()) file.close();
}

std::string ScriptManager::getScriptLine()
{
	return currentScriptLine->s_line;
}

std::string ScriptManager::getDisplayName()
{
	return currentScriptLine->name;
}

std::string ScriptManager::getBackgroundFileName() const
{
	return currentScriptLine->backgroundFileName;
}

std::string ScriptManager::getVoiceFileName() const
{
	return currentScriptLine->voiceFileName;
}

std::string ScriptManager::getBGMFileName() const
{
	return currentScriptLine->BGMFileName;
}

int ScriptManager::getNumChoices() const
{
	return currentScriptLine->numChoices;
}

bool ScriptManager::isChoice() const
{
	return currentScriptLine->isChoice;
}

std::vector<std::string> ScriptManager::getChoices() const
{
	return currentScriptLine->choices;
}

std::vector<std::string> ScriptManager::getNextFileNames() const
{
	return currentScriptLine->nextFileNames;
}

std::vector<int> ScriptManager::getNextLineIds() const
{
	return currentScriptLine->nextLineIDs;
}

int ScriptManager::getNumChars() const
{
	return currentScriptLine->numChars;
}

std::vector<CharPic> ScriptManager::getCharacterPicInfo() const
{
	return currentScriptLine->charPics;
}

void ScriptManager::init()
{
	currentScriptLine = new ScriptLine();
	file.open(filename);

	if (!file)
	{
		string err = "Cannot open script: " + filename;
		LOGGER->Log("ScriptManager", err);
		return;
	}

	UTILITY->skipFileLines(file, currentLineId-1);
	currentScriptLine->parse(file);
}

void ScriptManager::readNextLine()
{
	currentScriptLine->parse(file);
}

void ScriptManager::readLine(std::string filename, unsigned int lineId)
{
	if (this->filename != filename)
	{
		// open a new file
		this->filename = filename;
		file.close();
		file.open(filename);

		UTILITY->skipFileLines(file, lineId - 1);
	}
	else
	{
		if (lineId > currentLineId)
		{
			int diff = lineId - currentLineId;
			UTILITY->skipFileLines(file, diff - 1);
		}
		else
		{
			file.seekg(file.beg);
			UTILITY->skipFileLines(file, lineId - 1);
		}
	}

	currentLineId = lineId;
	currentScriptLine->parse(file);
}