#include "MenuButton.h"

bool MenuButton::onLoad()
{
	bool ret = Button::onLoad();
	return ret;
}

void MenuButton::onUpdate(float delta_t)
{
	Button::onUpdate(delta_t);
}

void MenuButton::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	Button::onDraw(target, states);
}

void MenuButton::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	Button::handleInput(e, window);
}
