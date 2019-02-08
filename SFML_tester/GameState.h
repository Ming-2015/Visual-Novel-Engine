#pragma once
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

class GameState {

public:
	virtual void handleInput(sf::Event e) = 0;			// handle user inputs (keyboard/gamepad/mouse/etc)
	virtual void render(sf::RenderWindow& window) = 0;	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	virtual void update() = 0;							// all non-rendering game logics/mechanics should be implemented here
	virtual void init() = 0;
};