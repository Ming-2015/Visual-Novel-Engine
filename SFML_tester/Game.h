#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>

#include "ScriptLine.h"
#include "Config.h"

class Game 
{
public:
	
	Game(Config config);					// initialize main game class
	
	void handleInput(sf::Event e);			// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update();							// all non-rendering game logics/mechanics should be implemented here

private:

	Config config;						// game configurations

	sf::Music bgm;						// BGM audio
	sf::Texture backgroundTex;			// background texture
	sf::Sprite background;				// background sprite
	sf::Music voice;					// voice audio

	int currentLineId;					// current line ID
	std::string currentFileName;		// current file name
	std::ifstream currentFileStream;	// current file stream

	sf::Text text;						// current text to be rendered
	sf::Font textFont;					// current text font used
	ScriptLine currentScriptLine;		// script line object that contains info on what need to be rendered
};