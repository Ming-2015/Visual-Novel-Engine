#pragma once
#include "GameState.h"
#include "Logger.h"

class InitState : public GameState
{
public: 
	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update();								// all non-rendering game logics/mechanics should be implemented here
	void init();

	const int myState = GameState::STATE_INIT;

	InitState();
};