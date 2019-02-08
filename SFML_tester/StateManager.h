#pragma once
#include "GameState.h"
#include "InitState.h"
#include "MenuState.h"

class StateManager 
{
public:
	GameState * currentState;
	int currentStateCode;

	StateManager();
	void handleInput(sf::Event& e, sf::RenderWindow& window);			// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update();							// all non-rendering game logics/mechanics should be implemented here
	void init();
};