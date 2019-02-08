#include "NewGameState.h"



NewGameState::NewGameState()
{
}


NewGameState::~NewGameState()
{
}

void NewGameState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
}

void NewGameState::render(sf::RenderWindow & window)
{
}

void NewGameState::update(float delta_t)
{
}

void NewGameState::init()
{
	nextState = GameState::STATE_MAIN;
	shouldChangeState = true;
	LOGGER->Log("NewGameState", "Switching to Main State");
}
