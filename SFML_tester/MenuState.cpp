#include "MenuState.h"
#include "Config.h"
#include "GameState.h"
#include "scriptLine.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

MenuState::MenuState() 
{
	init();

	ScriptLine testScript;
	ifstream myfile("resources/ScriptLine.csv");
	testScript.parse(myfile);
}

void MenuState::init() 
{

	if (!background.loadFromFile("assets/background.png"))
		LOGGER->Log("MenuState", "Image not found: background.png");
	backgroundImage.setTexture(background);

	if (!font.loadFromFile("assets/MATURASC.TTF"))
		LOGGER->Log("MenuState", "Can't find font file");

	testButtonObj = new MenuButton("assets/exitButton.png", "", "", 90.0f, 600.0f, 0, 0);
	testButtonObj->load();

	//THIS MIGHT NOT BE NEEDED, TESTING PURPOSES FOR NOW
	startText.setFont(font);
	startText.setStyle(sf::Text::Bold);
	startText.setString("Create New Game");
	startText.setFillColor(sf::Color::White);
	startText.setCharacterSize(48);
	startText.setPosition(50.0f, 320.0f);

	loadText.setFont(font);
	loadText.setStyle(sf::Text::Bold);
	loadText.setString("Load Game");
	loadText.setFillColor(sf::Color::White);
	loadText.setCharacterSize(48);
	loadText.setPosition(50.0f, 380.0f);

	settingsText.setFont(font);
	settingsText.setStyle(sf::Text::Bold);
	settingsText.setString("Settings");
	settingsText.setFillColor(sf::Color::White);
	settingsText.setCharacterSize(48);
	settingsText.setPosition(50.0f, 440.0f);

	
	exitText.setFont(font);
	exitText.setStyle(sf::Text::Bold);
	exitText.setString("Exit");
	exitText.setFillColor(sf::Color::White);
	exitText.setCharacterSize(48);
	exitText.setPosition(50.0f, 500.0f);
}

void MenuState::render(sf::RenderWindow& window) {
	window.draw(backgroundImage);
	window.draw(*testButtonObj);
	window.draw(startText);
	window.draw(loadText);
	window.draw(settingsText);
	window.draw(exitText);
}

void MenuState::update(float delta_t)
{
	testButtonObj->update(delta_t);
}

void MenuState::handleInput(sf::Event& e, sf::RenderWindow& window) {

	testButtonObj->handleInput(e, window);

	// if the button is being clicked
	if (testButtonObj->isClicked(true))
	{
		shouldChangeState = true;
		nextState = GameState::STATE_NEW_GAME;
		LOGGER->Log("MenuState", "Starting a new game");
	}
}

