#pragma once
#include "scriptLine.h"
#include "Utility.h"
#include "charPic.h"
#include "logger.h"
#include "GameState.h"
#include "ScriptLine.h"
#include "ScriptManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class SaveState : public GameState{
public:

	SaveState(std::string filename, unsigned int lineId)
		: scriptFileName(filename), lineId(lineId)
	{ 
	
	}

	~SaveState()
	{

	}

	int lineIDFromSave;
	string fileNameFromSave;
	void writeSave();
	void readSave();

	// handle user inputs (keyboard/gamepad/mouse/etc)
	void handleInput(sf::Event& e, sf::RenderWindow& window);

	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void render(sf::RenderWindow& window);

	// all non-rendering game logics/mechanics should be implemented here
	void update(float delta_t);

	// initialize instance
	void init();

	const int myState = GameState::STATE_NEW_GAME;

private:
	string scriptFileName;
	unsigned int lineId;
	string tempStr;

};