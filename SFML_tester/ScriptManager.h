#pragma once
#include <string>
#include <fstream>
#include <istream>
#include <cstddef>
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
	std::string getScriptLine() const;
	std::string getDisplayName() const;
	std::string getBackgroundFileName() const;
	bool getBackgroundChange() const;
	std::string getTextboxFileName() const;
	bool getTextboxChange() const;
	std::string getVoiceFileName() const;
	std::string getBGMFileName() const;
	int getNumChoices() const;
	bool isChoice() const;
	std::vector < std::string > getChoices() const;
	std::vector < std::string > getNextFileNames() const;
	std::vector < int > getNextLineIds() const;
	int getNumChars() const;
	std::vector < CharPic > getCharacterPicInfo() const;
	std::string getCurrentFileName() const;
	unsigned int getCurrentLineId() const;

	void readNextLine();
	void readLine(std::string filename, unsigned int lineId);
	void init();

	void addNewLineToPrevWord(unsigned int charLength);
	void addAllNewLines(unsigned int charLength, unsigned int lineLength);

	bool eof();

private:
	std::string filename;
	unsigned int currentLineId;

	ScriptLine * currentScriptLine;
	ifstream file;	
};