#include "Game.h"

Game::Game()
{
}

void Game::handleInput(sf::Event& e, sf::RenderWindow& window)
{
	stateManager.handleInput(e, window);
}

void Game::render(sf::RenderWindow & window)
{
	stateManager.render(window);
}

void Game::update()
{
	stateManager.update();
}
