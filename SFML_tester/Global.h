#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "Logger.h"
#include "Utility.h"

using namespace std;

#define GLOBAL Global::GetGlobal()

class Global {
public:

	// Define all the global variables to be passed between states
	std::string MAIN_STATE_currentFile;
	unsigned int MAIN_STATE_currentLineId;

	const std::string ResourceRoot = "resources/";
	const std::string ImageRoot = "images/";

	const std::string TextBoxLocation = "images/textbox/textboxw.png";
	const std::string TextBoxColorLocation = "resources/TextboxColors.csv";
	const std::string NewGameScriptFileLocation = "resources/ScriptLine.csv";
	const std::string DisplayTextFont = "assets/default.ttf";
	const std::string DisplayNameFont = "assets/MATURASC.TTF";
	
	string PlayerName = "PlayerName";
	bool autoMode = false;

	sf::Color getTextboxColor(const std::string& name);

	static Global* GetGlobal();
	static void CleanUp();

private:
	Global();
	~Global();
	void initTextboxColors(const std::string& filename);

	std::map<std::string, sf::Color> textBoxColors;

	static Global * global_ptr;
};
