#include "StateManager.h"

StateManager::StateManager()
{
	init();
}

void StateManager::handleInput(sf::Event& e, sf::RenderWindow& window)
{
	currentState->handleInput(e, window);
}

void StateManager::render(sf::RenderWindow & window)
{
	currentState->render(window);
}

void StateManager::update()
{
	currentState->update();
	if (currentState->shouldChangeState)
	{
		switch (currentState->nextState)
		{
			case GameState::STATE_MENU:
				delete currentState;
				currentState = new MenuState();
				break;
			case GameState::STATE_INIT:
				delete currentState;
				currentState = new InitState();
				break;
		}
	}
}

void StateManager::init()
{
	currentState = new InitState();
}

