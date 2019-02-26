#include "MainButton.h"

bool MainButton::onLoad()
{
	bool ret = Button::onLoad();
	sf::IntRect rectSourceSprite(startSpriteSheetXPos, startSpriteSheetYPos, endSpriteSheetXPos, endSpriteSheetYPos);
	sf::IntRect rectSourceSprite2(startSpriteSheetXPos2, startSpriteSheetYPos2, endSpriteSheetXPos2, endSpriteSheetYPos2);
	sprite.setTextureRect(rectSourceSprite);
	spriteHover.setTextureRect(rectSourceSprite2);
	scaleMultiply = 1;
	sprite.setOrigin(sf::Vector2f(texWidth / 2, texHeight / 2));
	spriteHover.setOrigin(sf::Vector2f(texWidth / 2, texHeight / 2));
	return ret;
}

void MainButton::onUpdate(float delta_t)
{
	Button::onUpdate(delta_t);
}

void MainButton::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	Button::onDraw(target, states);
}

void MainButton::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	Button::onHandleInput(e, window);

	switch (e.type)
	{
		case sf::Event::MouseMoved:
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			if (sprite.getGlobalBounds().contains(mousePosF))
			{
				hovered = true;
				//Show Sprite 2
			}
			else
			{
				hovered = false;
				//Do nothing
			}
		}
		break;
		case sf::Event::MouseButtonPressed:
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				if (sprite.getGlobalBounds().contains(mousePosF))
				{
					pressed = true;
				}
			}
		}
		break;
		case sf::Event::MouseButtonReleased:
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				if (pressed)
				{
					pressed = false;
					clicked = true;
				}
			}
		}
	}
}

void MainButton::setSpriteSheetPos(float startX, float startY, float endX, float endY)
{
	startSpriteSheetXPos = startX;
	startSpriteSheetYPos = startY;
	endSpriteSheetXPos = endX;
	endSpriteSheetYPos = endY;
}

void MainButton::setSpriteSheetPos2(float startX, float startY, float endX, float endY)
{
	startSpriteSheetXPos2 = startX;
	startSpriteSheetYPos2 = startY;
	endSpriteSheetXPos2 = endX;
	endSpriteSheetYPos2 = endY;
}

void MainButton::setAlpha(float a)
{
	alpha = a;
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	spriteHover.setColor(sf::Color(255, 255, 255, alpha));
}

void MainButton::addAlpha(float offset)
{
	alpha += offset;
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	spriteHover.setColor(sf::Color(255, 255, 255, alpha));
}
