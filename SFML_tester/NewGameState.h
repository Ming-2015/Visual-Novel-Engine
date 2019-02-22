#pragma once
#include "Global.h"
#include "Config.h"
#include "GameState.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "MenuButton.h"
using namespace std;

class NewGameState : public GameState
{
public:
	NewGameState();
	~NewGameState();
	
	// handle user inputs (keyboard/gamepad/mouse/etc)
	void handleInput(sf::Event& e, sf::RenderWindow& window);

	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void render(sf::RenderWindow& window);

	// all non-rendering game logics/mechanics should be implemented here
	void update(float delta_t);

	// initialize instance
	void init();	
	
	void cleanup();


private:
	sf::Music bgm;
	sf::Texture background;
	sf::Sprite backgroundImage;
	sf::Font font;

	sf::RectangleShape rectangle;
	sf::Clock clock;
	
	MenuButton * submitButton;

	bool shouldFade = true;
	bool shouldFadeOut = false;
	int currentAlpha;
	int endAlpha;
	float volumeFade;
	float masterVolume;
	float bgmVolume;

	sf::String playerInput;
	sf::Text playerText;

	bool clickedSubmit = false;
};

