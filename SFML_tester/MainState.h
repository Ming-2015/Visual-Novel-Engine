#pragma once
#include "GameState.h"
#include "ScriptLine.h"
#include "ScriptManager.h"
#include "Global.h"
#include "Fade.h"
#include "DrawMainButton.h"
#include "ReturnMenuPrompt.h"
#include "SavedataUtility.h"

#include <stdio.h>

class MainState : public GameState
{
public:

	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);								// all non-rendering game logics/mechanics should be implemented here
	void init();
	void cleanup();

	const ScriptManager* getScriptManager();

	MainState();
	MainState(std::string savefile);	// for loading the main state with a save file
	~MainState();

private:

	ScriptManager* scriptManager = nullptr;
	DrawMainButton* drawMainButton = nullptr;
	ReturnMenuPrompt* returnMenuPrompt = nullptr;
};