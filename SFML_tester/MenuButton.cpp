#include "MenuButton.h"

bool MenuButton::onLoad()
{
	bool ret = Button::onLoad();
	scaleMultiply = 1.0;
	sprite.setOrigin(sf::Vector2f( texWidth / 2, texHeight / 2));
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
	Button::onHandleInput(e, window);

	switch (e.type)
	{
	case sf::Event::Closed:
		window.close();
		break;

	case sf::Event::MouseMoved:
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		if (sprite.getGlobalBounds().contains(mousePosF))
		{
			if (clock.getElapsedTime().asMilliseconds() > 100.00f) {
				while (scaleMultiply < 1.1) {
					sprite.setScale(sf::Vector2f((1.0f*scaleMultiply), (1.0f*scaleMultiply)));
					scaleMultiply += 0.05;
				}
				clock.restart();
			}
		}
		else
		{
			sprite.setScale(sf::Vector2f(1.0f, 1.0f));
			scaleMultiply = 1.0;
		}
	}
	break;
	case sf::Event::MouseButtonPressed:
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		if (sprite.getGlobalBounds().contains(mousePosF))
		{
			LOGGER->Log("MenuButton", "Exit Button test");
			pressed = true;
		}
	}
	break;
	case sf::Event::MouseButtonReleased:
		if (pressed)
		{
			pressed = false;
			clicked = true;
		}
	}
}
