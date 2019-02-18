#pragma once
#include <string>
#include <fstream>
#include <istream>
#include "Utility.h"
#include "ScriptLine.h"
#include "ItemImage.h"
#include "ScriptCommand.h"
#include "ShowCommand.h"
#include "DisplayCommand.h"
#include "SetCommand.h"
#include "MoveCommand.h"
#include "RotateCommand.h"
#include "ZoomCommand.h"
#include "PlayCommand.h"
#include "StopCommand.h"
#include "PauseCommand.h"
#include "ResumeCommand.h"
#include "RemoveCommand.h"
#include "ClearCommand.h"
#include "HideCommand.h"
#include "DelayCommand.h"
#include "UnhideCommand.h"

// This class is for keeping a Script
class ScriptManager
{
public:
	ScriptManager(std::string filename);
	~ScriptManager();

	// get the current line info
	std::string getScriptLine() const;
	std::string getDisplayName() const;
	const TextboxImage* getTextboxImage() const;
	bool shouldHideTextbox() const;

	std::string getBGMFileName() const;

	int getNumChoices() const;
	bool isChoice() const;
	std::vector < std::string > getChoices() const;
	std::vector < std::string > getNextFileNames() const;

	std::vector < CharacterImage* > getCharacterImages() const;
	std::vector < BackgroundImage* > getBackgroundImages() const;
	std::string getCurrentFileName() const;
	
	bool eof() const;
	bool doneAllCommands() const;

	void readCommands();
	void readNewFile(std::string filename);

	void init();
	void update(float delta_t);
	void handleInput(sf::Event & e, sf::RenderWindow & window);

private:
	std::vector< ScriptCommand* > commands;
	ScriptLine * currentScriptLine;

	std::string initFileName;
};