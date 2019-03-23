#pragma once

#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "scriptLine.h"
#include "Utility.h"
#include "logger.h"
#include "GameState.h"
#include "ScriptLine.h"
#include "ScriptManager.h"
#include "Global.h"
#include "SavefileImage.h"
#include "SavedataUtility.h"

using namespace std;

class SaveState : public GameState{
public:

	SaveState( const ScriptManager* scriptManager, sf::Image screenshot );
	~SaveState();

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
	sf::Texture saveTexture;
	sf::Sprite saveBackground;
	sf::Font settingsFont;
	sf::Text returnState;

	unsigned int currentPageNumber = 0;
	const unsigned int savePerPage = 8;

	std::vector<SavefileImage *> savefileImages;

	const ScriptManager * scriptManager;
	sf::Image screenshot;

	void loadSavesByPage(int pageNumber);

	const static int INDEX_SAVE_1 = 0;
	const static int INDEX_SAVE_2 = 1;
	const static int INDEX_SAVE_3 = 2;
	const static int INDEX_SAVE_4 = 3;
	const static int INDEX_SAVE_5 = 4;
	const static int INDEX_SAVE_6 = 5;
};