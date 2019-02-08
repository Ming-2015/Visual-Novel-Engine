#include "InitState.h"

void InitState::handleInput(sf::Event & e, sf::RenderWindow & window)
{

}

void InitState::render(sf::RenderWindow & window)
{

}

void InitState::update()
{

}

void InitState::init()
{
	nextState = GameState::STATE_MENU;
	shouldChangeState = true;
	LOGGER->Log("InitState", "Switching to Menu State");
}

InitState::InitState()
{
	init();
}
