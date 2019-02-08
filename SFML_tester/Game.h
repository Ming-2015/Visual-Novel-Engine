#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>

#include "Config.h"
#include "StateManager.h"

class Game 
{
public:
	
	Game();									// initialize main game class
	
	void handleInput(sf::Event& e, sf::RenderWindow& window);			// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);							// all non-rendering game logics/mechanics should be implemented here

private:

	StateManager stateManager;
};