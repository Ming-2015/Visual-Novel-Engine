#include "ExitState.h"

void ExitState::handleInput(sf::Event & e, sf::RenderWindow & window)
{

}

void ExitState::render(sf::RenderWindow & window)
{
	window.close();
}

void ExitState::update(float delta_t)
{

}

void ExitState::init()
{
	
}

void ExitState::cleanup()
{
}

ExitState::ExitState()
{
	myState = GameState::STATE_EXIT;
	init();
}

ExitState::~ExitState()
{
}
