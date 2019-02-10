#include "StateManager.h"

StateManager::~StateManager()
{
	if (currentState != nullptr)
		delete currentState;
}

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

void StateManager::update(float delta_t)
{
	currentState->update(delta_t);
	manageStates();
}

void StateManager::init()
{
	currentState = new InitState();
}

void StateManager::manageStates()
{
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
		case GameState::STATE_NEW_GAME:
			delete currentState;
			currentState = new NewGameState();
			break;
		case GameState::STATE_MAIN:
			delete currentState;
			currentState = new MainState("resources/ScriptLine.csv", 1);
			break;
		case GameState::STATE_SAVE:
			prevStates.push(currentState);
			currentState->shouldChangeState = false;
			currentState = new SaveState(GLOBAL->MAIN_STATE_currentFile, GLOBAL->MAIN_STATE_currentLineId);
			break;
		case GameState::STATE_EXIT:
			delete currentState;
			currentState = new ExitState();
			break;
		case GameState::STATE_CONFIG:
			prevStates.push(currentState);
			currentState->shouldChangeState = false;
			currentState = new SettingsState();
			break;
		case GameState::STATE_BACK:
			delete currentState;
			currentState = prevStates.top();
			prevStates.pop();
			break;
		}
		currentState->init();
	}
}

