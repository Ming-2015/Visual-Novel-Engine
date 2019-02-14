#include "LoadState.h"

LoadState::LoadState()
{

}

LoadState::~LoadState()
{

}

void LoadState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		if (returnState.getGlobalBounds().contains(mousePosF))
		{
			nextState = GameState::STATE_BACK;
			shouldChangeState = true;
			LOGGER->Log("SaveState", "Returning to Main Menu");
		}
		if (startNew.getGlobalBounds().contains(mousePosF))
		{
			shouldChangeState = true;
			//bgm.stop(); Music Still Plays
			nextState = GameState::STATE_NEW_GAME;
			LOGGER->Log("SaveState", "Starting a new game");
		}
		break;
	}
	}
}

void LoadState::render(sf::RenderWindow & window)
{
	window.draw(saveBackground);
	window.draw(startNew);
	window.draw(returnState);
}

void LoadState::update(float delta_t)
{

}

void LoadState::init()
{
	if (!saveTexture.loadFromFile("assets/LoadPage.png"))
		LOGGER->Log("MenuState", "Image not found: LoadPage.png");
	saveBackground.setTexture(saveTexture);

	if (!settingsFont.loadFromFile("assets/MATURASC.TTF"))
	{
		LOGGER->Log("SettingsState", "Unable to find default font");
	}

	startNew.setFont(settingsFont);
	startNew.setString("Start New Story");
	startNew.setFillColor(sf::Color::White);
	startNew.setCharacterSize(42);
	startNew.setPosition(380.0f, 790.0f);

	returnState.setFont(settingsFont);
	returnState.setString("Return to Menu");
	returnState.setFillColor(sf::Color::White);
	returnState.setCharacterSize(42);
	returnState.setPosition(880.0f, 790.0f);
}