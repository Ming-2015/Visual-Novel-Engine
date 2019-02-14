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

std::string ScriptManager::getScriptLine() const
{
	return currentScriptLine->s_line;
}

std::string ScriptManager::getDisplayName() const 
{
	return currentScriptLine->name;
}

bool ScriptManager::getBackgroundChange() const 
{
	return currentScriptLine->backgroundChange;
}

std::string ScriptManager::getBackgroundFileName() const
{
	return currentScriptLine->backgroundFileName;
}

bool ScriptManager::getTextboxChange() const
{
	return currentScriptLine->textboxChange;
}

std::string ScriptManager::getTextboxFileName() const
{
	return currentScriptLine->textboxFileName;
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

std::string ScriptManager::getCurrentFileName() const
{
	return std::string();
}

unsigned int ScriptManager::getCurrentLineId() const
{
	return currentScriptLine->currentLineID;
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

void ScriptManager::addNewLineToPrevWord(unsigned int charLength)
{
	unsigned int found = UTILITY->findLastOf(currentScriptLine->s_line, ' ', charLength);
	currentScriptLine->s_line = currentScriptLine->s_line.substr(0, found+1) + "\n" 
		+ currentScriptLine->s_line.substr(found+1, currentScriptLine->s_line.length() - found + 1);
}

void ScriptManager::addAllNewLines(unsigned int charLength, unsigned int lineLength)
{
	int currentChar = charLength;
	currentChar = (currentChar / lineLength + 1) * lineLength;
	while (currentChar < currentScriptLine->s_line.length())
	{
		addNewLineToPrevWord(currentChar);
		currentChar += lineLength;
	}
}

bool ScriptManager::eof()
{
	return file.eof();
}

void ScriptManager::readNextLine()
{
	if (!file.eof())
	{
		currentLineId++;
		currentScriptLine->parse(file);
	}
	else
	{
		LOGGER->Log("ScriptManager", "Reached EOF of current Script File!");
	}
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

	if (!file.eof() && file)
	{
		currentLineId = lineId;
		currentScriptLine->parse(file);
	}
	else
	{
		LOGGER->Log("ScriptManager", "Invalid combination of Script File and Line ID");
	}
}