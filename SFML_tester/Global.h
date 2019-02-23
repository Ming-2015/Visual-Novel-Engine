#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <SFML/Graphics.hpp>

#include "Logger.h"
#include "Utility.h"

using namespace std;

#define GLOBAL Global::GetGlobal()

class ScriptManager;
class Global {
public:

	// Define all the global variables to be passed between states
	std::string selectedLoadFile = NewGameScriptFileLocation;
	
	const std::string ResourceRoot = "resources/";
	const std::string ImageRoot = "images/";

	const std::string TextBoxLocation = "images/textbox/textboxMaster.png";
	const std::string TextBoxColorLocation = "resources/TextboxColors.csv";
	const std::string NewGameScriptFileLocation = "resources/ScriptLine.csv";

	std::string DisplayTextFont = "resources/default.ttf";
	std::string DisplayNameFont = "resources/MATURASC.TTF";
	std::string UserInterfaceFont = "resources/default.TTF";
	std::string UserInterfaceButtonFont = "resources/MATURASC.TTF";

	const std::string blurFragShaderPath = "resources/blur.frag";

	const std::string SavefileRoot = "saves/";
	const std::string SavefilePrefix = "save_";
	const std::string SavefileSuffix = ".usd";

	std::string playerName;

	bool autoMode = false;
	bool skipMode = false;

	sf::RenderWindow* windowPtr = nullptr;
	ScriptManager* scriptManagerPtr = nullptr;

	sf::Color choiceboxColorSelected = sf::Color(255, 20, 150, 190);
	sf::Color choiceboxColorUnselected = sf::Color(180, 0, 80, 190);

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
