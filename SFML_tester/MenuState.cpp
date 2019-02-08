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
		cout << "Image not found: " << "background.png" << endl;
	backgroundImage.setTexture(background);

	if (!font.loadFromFile("assets/MATURASC.TTF"))
		cout << "Can't find font file" << endl;

	//if (!testButton.loadFromFile("assets/exitButton.png"))
	//	cout << "Image no found" << endl;
	//testButtonPNG.setPosition(50.0f, 600.0f);

	//float exitButtonWidth = testButtonPNG.getLocalBounds().width;
	//float exitButtonHeight = testButtonPNG.getLocalBounds().height;

	//testButtonPNG.setTexture(testButton);

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
}

void MenuState::handleInput(sf::Event& e, sf::RenderWindow& window) {
	/*
	switch (e.type)
	{
		case sf::Event::MouseMoved:
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			if (testButtonObj->getSprite().getGlobalBounds().contains(mousePosF))
			{
				testButtonObj->getSprite().setColor(sf::Color(80, 15, 176));
				LOGGER->Log("MenuState", "Hover over Exit Button!");
			}
			else
			{
				testButtonObj->getSprite().setColor(sf::Color(255, 100, 255));
			}

		}
		break;
		case sf::Event::MouseButtonPressed:
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			if (testButtonObj->getSprite().getGlobalBounds().contains(mousePosF))
			{
				LOGGER->Log("MenuState", "Exit Button Pressed!");
			}
		}
		break;
	}*/
	testButtonObj->onHandleInput(e, window);
}

