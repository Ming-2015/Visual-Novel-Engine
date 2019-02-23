#include "MenuState.h"
#include "Config.h"
#include "GameState.h"
#include "scriptLine.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

MenuState::MenuState() 
{
	myState = GameState::STATE_MENU;
	init();
}

MenuState::~MenuState()
{
}

void MenuState::init() 
{
	currentAlpha = 0;
	endAlpha = 255;
	rectangle.setSize(sf::Vector2f(1600, 900));
	rectangle.setFillColor(sf::Color::Color(0,0,0,currentAlpha));
	rectangle.setPosition(0, 0);

	volumeFade = 0.0;
	masterVolume = CONFIG->masterVolume;
	bgmVolume = CONFIG->bgmVolume;
	bgm.setVolume(100.0f*bgmVolume*masterVolume);
	if (bgm.getStatus() != sf::Music::Playing) {
		if (!bgm.openFromFile(GLOBAL->MusicRoot + "HGSSRoute47.WAV"))
			LOGGER->Log("MenuState", "BGM not found!");
		bgm.play();
	}

	if (!background.loadFromFile(GLOBAL->AssetRoot + "background.jpg"))
		LOGGER->Log("MenuState", "Image not found: background.jpg");
	backgroundImage.setTexture(background);

	if (!font.loadFromFile(GLOBAL->UserInterfaceButtonFont))
		LOGGER->Log("MenuState", "Can't find font file");

	startButton = new MenuButton(GLOBAL->AssetRoot + "MenuButtons600wx288h3.png", "", "", 350.0f, 520.0f, 0, 0, 0, 0, 475, 72);
	startButton->load();

	loadButton = new MenuButton(GLOBAL->AssetRoot + "MenuButtons600wx288h3.png", "", "", 350.0f, 610.0f, 0, 0, 0, 72, 335, 72);
	loadButton->load();

	settingsButton = new MenuButton(GLOBAL->AssetRoot + "MenuButtons600wx288h3.png", "", "", 350.0f, 700.0f, 0, 0, 0, 144, 225, 72);
	settingsButton->load();

	exitButton = new MenuButton(GLOBAL->AssetRoot + "MenuButtons600wx288h3.png", "", "", 350.0f, 790.0f, 0, 0, 0, 216, 125, 72);
	exitButton->load();

	//THIS MIGHT NOT BE NEEDED, TESTING PURPOSES FOR NOW
	/*
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
	*/
}

void MenuState::cleanup()
{
	bgm.stop();
	delete startButton;
	delete loadButton;
	delete settingsButton;
	delete exitButton;
}

void MenuState::render(sf::RenderWindow& window) {
	window.draw(backgroundImage);
	window.draw(*startButton);
	window.draw(*loadButton);
	window.draw(*settingsButton);
	window.draw(*exitButton);
	window.draw(rectangle);
	//window.draw(startText);
	//window.draw(loadText);
	//window.draw(settingsText);
	//window.draw(exitText);
}

void MenuState::update(float delta_t)
{
	exitButton->update(delta_t);
	startButton->update(delta_t);
	settingsButton->update(delta_t);
	loadButton->update(delta_t);

	if ((masterVolume != CONFIG->masterVolume || bgmVolume != CONFIG->bgmVolume) && shouldFade == false)
	{
		masterVolume = CONFIG->masterVolume;
		bgmVolume = CONFIG->bgmVolume;
		bgm.setVolume(100.0*bgmVolume*masterVolume);
	}

	if (shouldFade && isExit == true) 
	{
		if (clock.getElapsedTime().asMilliseconds() > 20.0f)
		{
			if (currentAlpha < endAlpha) 
			{
				currentAlpha += 10;
				rectangle.setFillColor(sf::Color::Color(0, 0, 0, currentAlpha));
				
				volumeFade += 4.0f;
				float volume = 100.0f*bgmVolume*masterVolume - volumeFade;
		
				if (currentAlpha >= endAlpha)
				{
					currentAlpha = endAlpha;
				}
				if (volume <= 0.0)
				{
					volume = 0;
				}
				bgm.setVolume(volume);
			}
			else 
			{
				shouldChangeState = true;
				bgm.stop();
				nextState = GameState::STATE_EXIT;
			}
			clock.restart();
		}
	}


	if (shouldFade && isExit == false) 
	{
		if (clock.getElapsedTime().asMilliseconds() > 20.0f) 
		{
			if (currentAlpha < endAlpha) {
				currentAlpha += 10;
				rectangle.setFillColor(sf::Color::Color(0, 0, 0, currentAlpha));

				volumeFade += 4.0f;
				float volume = 100.0f*bgmVolume*masterVolume - volumeFade;
				
				if (currentAlpha >= endAlpha)
				{
					currentAlpha = endAlpha;
				}
				if (volume <= 0.0)
				{
					volume = 0;
				}
				bgm.setVolume(volume);
			}
			else 
			{
				shouldChangeState = true;
				nextState = GameState::STATE_NEW_GAME;
				bgm.stop();
			}
			clock.restart();
		}
	}
	
	
}

void MenuState::handleInput(sf::Event& e, sf::RenderWindow& window) {

	exitButton->handleInput(e, window);
	startButton->handleInput(e, window);
	settingsButton->handleInput(e, window);
	loadButton->handleInput(e, window);

	// if the button is being clicked
	if (startButton->isClicked(true) && fadeTicked == false)
	{
		fadeTicked = true;
		currentAlpha = 5;
		shouldFade = true;
		isExit = false;
		LOGGER->Log("MenuState", "Starting a new game");
	}
	if (exitButton->isClicked(true) && fadeTicked == false)
	{
		fadeTicked = true;
		currentAlpha = 5;
		shouldFade = true;
		isExit = true;
		LOGGER->Log("MenuState", "Switching to Exit State");
	}
	if (settingsButton->isClicked(true))
	{
		shouldChangeState = true;
		nextState = GameState::STATE_CONFIG;
		LOGGER->Log("MenuState", "Opening settings");
	}
	if (loadButton->isClicked(true))
	{
		shouldChangeState = true;
		nextState = GameState::STATE_LOAD;
		LOGGER->Log("MenuState", "Opening Load States");
	}
}

