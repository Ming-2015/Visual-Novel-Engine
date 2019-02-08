#pragma once
#include <string>
#include <fstream>
#include <istream>
#include "Utility.h"
#include "ScriptLine.h"
#include "CharPic.h"

// This class is for keeping a Script
class ScriptManager
{
public:
	ScriptManager(std::string filename, unsigned int initLineId);
	~ScriptManager();

	// get the current line info
	std::string getScriptLine();
	std::string getDisplayName();

	std::string getBackgroundFileName() const;
	std::string getVoiceFileName() const;
	std::string getBGMFileName() const;
	int getNumChoices() const;
	bool isChoice() const;
	std::vector < std::string > getChoices() const;
	std::vector < std::string > getNextFileNames() const;
	std::vector < int > getNextLineIds() const;
	int getNumChars() const;
	std::vector < CharPic > getCharacterPicInfo() const;

	void readNextLine();
	void readLine(std::string filename, unsigned int lineId);

private:
	std::string filename;
	unsigned int currentLineId;

	ScriptLine * currentScriptLine;
	ifstream file;	
	
	void init();
};