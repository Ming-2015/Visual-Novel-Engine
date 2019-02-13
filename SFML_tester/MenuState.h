#pragma once
#include "Config.h"
#include "GameState.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "MenuButton.h"

class MenuState : public GameState
{
public:
	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);					// all non-rendering game logics/mechanics should be implemented here
	void init();

	MenuState();

	const int myState = GameState::STATE_MENU;

private:
	sf::Music bgm;
	sf::Texture background;
	sf::Sprite backgroundImage;
	sf::Font font;
	//sf::Text startText;
	//sf::Text loadText;
	//sf::Text settingsText;
	//sf::Text exitText;

	MenuButton * startButton;
	MenuButton * loadButton;
	MenuButton * settingsButton;
	MenuButton * exitButton;

	sf::RectangleShape rectangle;
	sf::Clock clock;

	bool shouldFade = false;
	int currentAlpha;
	int endAlpha;
	float volumeFade;
	float masterVolume;
	float bgmVolume;
};