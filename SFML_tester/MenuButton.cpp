#include "MenuButton.h"

bool MenuButton::onLoad()
{
	return Button::onLoad();
}

void MenuButton::onUpdate(float delta_t)
{
	Button::onUpdate(delta_t);
}

void MenuButton::onDraw(sf::RenderTarget & target, sf::RenderStates states)
{
	Button::onDraw(target, states);
}

void MenuButton::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	Button::handleInput(e, window);
}
