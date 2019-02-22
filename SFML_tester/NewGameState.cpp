#include "NewGameState.h"



NewGameState::NewGameState()
{
	myState = GameState::STATE_NEW_GAME;
	init();
}


NewGameState::~NewGameState()
{

}

void NewGameState::init()
{
	if (!background.loadFromFile("assets/NewGameBackground.png"))
		LOGGER->Log("NewGameState", "Image not found: NewGameBackground.png");
	backgroundImage.setTexture(background);

	submitButton = new MenuButton("assets/SubmitButton141x42.png", "", "", 787.0f, 590.0f, 0, 0, 0, 0, 141, 42);
	submitButton->load();

	currentAlpha = 0;
	endAlpha = 255;
	rectangle.setSize(sf::Vector2f(1600, 900));
	rectangle.setFillColor(sf::Color::Color(0, 0, 0, currentAlpha));
	rectangle.setPosition(0, 0);

	masterVolume = CONFIG->masterVolume;
	bgmVolume = CONFIG->bgmVolume;
	bgm.setVolume(100.0*bgmVolume*masterVolume);
	if (bgm.getStatus() != sf::Music::Playing) {
		if (!bgm.openFromFile("assets/NewGameState.WAV"))
			LOGGER->Log("NewGameState", "BGM not found!");
		bgm.play();
	}
	volumeFade = 0.0;

	if (!font.loadFromFile("assets/default.TTF"))
	{
		LOGGER->Log("SettingsState", "Unable to find default font");
	}

	playerText.setFont(font);
	playerText.setCharacterSize(50);
	playerText.setPosition(785, 375);
	playerText.setFillColor(sf::Color::White);
}

void NewGameState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	submitButton->handleInput(e, window);

	if (e.type == sf::Event::TextEntered)
	{
		if (e.text.unicode < 128)
		{
			if (playerInput.getSize() < 24)
			{
				if (e.text.unicode == 13)
				{
					if (playerInput.getSize() > 0)
					{
						GLOBAL->playerName = playerInput;
						currentAlpha = 5;
						shouldFadeOut = true;
						clickedSubmit = true;
						LOGGER->Log("NewGameState", "Switching to Main State");
					}
				}
				else if (e.text.unicode == 8)	//backspace
				{
					if (playerInput.getSize() > 0)
					{
						playerInput.erase(playerInput.getSize() - 1);
						playerText.setString(playerInput);
					}
				}
				else
				{
					playerInput += e.text.unicode;
					playerText.setString(playerInput);
				}
			}
			else
			{
				if (e.text.unicode == 8)	//backspace
				{
					if (playerInput.getSize() > 0)
					{
						playerInput.erase(playerInput.getSize() - 1);
						playerText.setString(playerInput);
					}
				}
				//give an error
			}
		}
	}

	if (submitButton->isClicked(true) && clickedSubmit == false)
	{
		if (playerInput.getSize() > 0)
		{
			clickedSubmit = true;
			GLOBAL->playerName = playerInput;
			currentAlpha = 5;
			shouldFadeOut = true;
			LOGGER->Log("NewGameState", "Switching to Main State");
		}
	}
	
}

void NewGameState::update(float delta_t)
{
	submitButton->update(delta_t);

	playerText.setOrigin((playerText.getLocalBounds().width / 2), (playerText.getLocalBounds().height / 2));
	playerText.setPosition(785, 375);

	if (masterVolume != CONFIG->masterVolume || bgmVolume != CONFIG->bgmVolume)
	{
		masterVolume = CONFIG->masterVolume;
		bgmVolume = CONFIG->bgmVolume;
		bgm.setVolume(100.0*bgmVolume*masterVolume);
	}

	if (shouldFadeOut)
	{
		if (clock.getElapsedTime().asMilliseconds() > 20.0f)
		{
			if (currentAlpha < endAlpha) {
				currentAlpha += 10;
				rectangle.setFillColor(sf::Color::Color(0, 0, 0, currentAlpha));

				volumeFade += 4;
				float volume = 100.0*bgmVolume*masterVolume - volumeFade;
				bgm.setVolume(volume);
			}
			else
			{
				bgm.stop();
				shouldChangeState = true;
				nextState = GameState::STATE_MAIN;
			}
			clock.restart();
		}
	}
}

void NewGameState::render(sf::RenderWindow & window)
{
	window.draw(backgroundImage);
	window.draw(*submitButton);
	window.draw(playerText);
	window.draw(rectangle);
}

void NewGameState::cleanup()
{
	bgm.stop();
	delete submitButton;
}
