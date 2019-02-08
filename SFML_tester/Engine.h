#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Config.h"

class Engine 
{
public:
	int start();	// start the program
	Engine();		// constructor

private:
	sf::RenderWindow window;	// the window to be rendered
};