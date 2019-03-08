#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Game.h"
#include "Config.h"
#include "SavedataUtility.h"
#include <math.h>

class Engine 
{
public:
	int start();	// start the program
	Engine();		// constructor

private:
	sf::RenderWindow window;	// the window to be rendered

	sf::Clock gameClock;
	sf::Time prevTime;
	float delta_t;
	float fps;
	std::string fpsStr;
	sf::Text fpsText;
	sf::Font fpsFont;
	bool displayFPS = true;

	void initFPSText();
	void initWindowSize(sf::RenderWindow& window);
};