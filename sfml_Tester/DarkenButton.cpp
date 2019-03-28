#include "DarkenButton.h"

bool DarkenButton::onLoad()
{
	bool ret = Button::onLoad();
	sf::IntRect rectSourceSprite(startSpriteSheetXPos, startSpriteSheetYPos, endSpriteSheetXPos, endSpriteSheetYPos);
	sprite.setTextureRect(rectSourceSprite);
	scaleMultiply = 1;
	highlight = false;
	sprite.setOrigin(sf::Vector2f(texWidth / 2, texHeight / 2));
	return ret;
}

void DarkenButton::onUpdate(float delta_t)
{
	Button::onUpdate(delta_t);

	if (highlight == true)
	{
		sprite.setColor(sf::Color(150, 150, 150, 255));
	}
	if (highlight == false)
	{
		sprite.setColor(sf::Color(255, 255, 255, 255));
	}
	if (hoverlight == true)
	{
		sprite.setColor(sf::Color(150, 150, 150, 255));
	}
}

void DarkenButton::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	Button::onDraw(target, states);
}

void DarkenButton::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	Button::onHandleInput(e, window);

	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
	switch (e.type)
	{
	case sf::Event::MouseMoved:
	{
		if (sprite.getGlobalBounds().contains(mousePosF))
		{
			hoverlight = true;
		}
		else
		{
			hoverlight = false;
		}
		break;
	}
	case sf::Event::MouseButtonPressed:
	{
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			if (sprite.getGlobalBounds().contains(mousePosF))
			{
				pressed = true;
			}
		}
		break;
	}
	case sf::Event::MouseButtonReleased:
	{
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			if (pressed)
			{
				pressed = false;
				clicked = true;
			}
			break;
		}
	}
	}
}

void DarkenButton::setSpriteSheetPos(float startX, float startY, float endX, float endY)
{
	startSpriteSheetXPos = startX;
	startSpriteSheetYPos = startY;
	endSpriteSheetXPos = endX;
	endSpriteSheetYPos = endY;
}
