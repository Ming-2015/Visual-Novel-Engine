#pragma once
#include "scriptLine.h"
#include "Utility.h"
#include "logger.h"
#include "GameState.h"
#include "ScriptLine.h"
#include "ScriptManager.h"
#include "Global.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

class SaveState : public GameState{
public:

	SaveState( const ScriptManager* scriptManager, sf::Image screenshot );

	~SaveState();

	int lineIDFromSave;
	string fileNameFromSave;

	void writeSave(const std::string& filename) const;

	// handle user inputs (keyboard/gamepad/mouse/etc)
	void handleInput(sf::Event& e, sf::RenderWindow& window);

	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void render(sf::RenderWindow& window);

	// all non-rendering game logics/mechanics should be implemented here
	void update(float delta_t);

	// initialize instance
	void init();

	void cleanup();

private:

	const ScriptManager * scriptManager;
	sf::Image screenshot;
};