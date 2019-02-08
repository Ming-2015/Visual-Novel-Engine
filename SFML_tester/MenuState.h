#pragma once
#include "Config.h"
#include "GameState.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

class MenuState : public GameState
{
public:
	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update();								// all non-rendering game logics/mechanics should be implemented here
	void init();

	MenuState();

	const int myState = GameState::STATE_MENU;

private:
	sf::Texture background;
	sf::Sprite backgroundImage;
	sf::Font font;
	sf::Texture testButton;
	sf::Sprite testButtonPNG;
	sf::Text startText;
	sf::Text loadText;
	sf::Text settingsText;
	sf::Text exitText;
};