#pragma once
#include <string>
#include <fstream>
#include <istream>
#include "Utility.h"
#include "ScriptLine.h"
#include "Linelog.h"
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
#include "FlashCommand.h"
#include "HideCommand.h"
#include "DelayCommand.h"
#include "UnhideCommand.h"
#include "JumpCommand.h"
#include "BlurCommand.h"
#include "StartLoopCommand.h"
#include "StopLoopCommand.h"

// This class is for keeping a Script
class ScriptManager
{
public:
	ScriptManager(std::string filename);
	~ScriptManager();

	ScriptManager(ifstream& file);
	void serialize(ofstream& savefile) const;

	// get the current line info
	std::string getScriptLine() const;
	std::string getDisplayName() const;
	const TextboxImage* getTextboxImage() const;
	bool shouldHideTextbox() const;

	std::vector<ChoiceImage*> getChoices() const;
	bool isChoice() const;

	std::vector < CharacterImage* > getCharacterImages() const;
	std::vector < BackgroundImage* > getBackgroundImages() const;
	std::string getCurrentFileName() const;
	
	bool eof() const;
	bool doneAllCommands() const;

	void readCommands();

	void init();
	void update(float delta_t);
	void handleInput(sf::Event & e, sf::RenderWindow & window);

	void setPlayerName(const std::string& name);
	std::string getPlayerName() const;

	void hideTextbox();
	void showTextbox();
	bool isTextboxClosed();

	LineLog* getLineLog() const;
	CinematicBars* getCinematicBars() const;

private:

	std::vector< ScriptCommand* > commands;
	ScriptLine * currentScriptLine;

	std::string initFileName;
	bool shouldCloseTextbox = false;

	float s_masterVolume = CONFIG->masterVolume;
	float s_bgmVolume = CONFIG->bgmVolume;
	float s_sfxVolume = CONFIG->sfxVolume;
	float s_voiceVolume = CONFIG->voiceVolume;
	float s_textWindowAlpha = CONFIG->textWindowAlpha;

	void advanceText();

	LineLogItem logItem;
	std::string logVoicefile; 
};