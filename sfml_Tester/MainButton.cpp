#include "MainButton.h"

bool MainButton::onLoad()
{
	bool ret = Button::onLoad();
	sf::IntRect rectSourceSprite(startSpriteSheetXPos, startSpriteSheetYPos, endSpriteSheetXPos, endSpriteSheetYPos);
	sf::IntRect rectSourceSprite2(startSpriteSheetXPos2, startSpriteSheetYPos2, endSpriteSheetXPos2, endSpriteSheetYPos2);
	
	// original sprite is automatically set with the texture
	sprite.setTextureRect(rectSourceSprite);
	
	// new sprite doesn't have the hover sprite
	spriteHover.setTexture(texture);
	spriteHover.setPosition(xPos, yPos);
	spriteHover.setTextureRect(rectSourceSprite2);

	scaleMultiply = 1;
	sprite.setOrigin(sf::Vector2f(texWidth / 2, texHeight / 2));
	spriteHover.setOrigin(sf::Vector2f(texWidth / 2, texHeight / 2));

	return ret;
}

void MainButton::onUpdate(float delta_t)
{
	Button::onUpdate(delta_t);

	highlightColor.a = alpha;
	if (highlight && !hovered)
	{
		spriteHover.setColor(highlightColor);
	}
	else
	{
		spriteHover.setColor(defaultColor);
	}
}

void MainButton::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (hasShader)
	{
		states.shader = &shader;
	}

	if (hovered || highlight)
	{
		target.draw(spriteHover, states);
	}
	else
	{
		target.draw(sprite, states);
	}
}

void MainButton::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	Button::onHandleInput(e, window);

	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);

	switch (e.type)
	{
		case sf::Event::MouseMoved:
		{
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
	defaultColor.a = alpha;
	highlightColor.a = alpha;
	sprite.setColor(defaultColor);
	spriteHover.setColor( highlight && !hovered ? highlightColor : defaultColor);
}

void MainButton::addAlpha(float offset)
{
	alpha += offset;
	defaultColor.a = alpha;
	highlightColor.a = alpha;
	sprite.setColor(defaultColor);
	spriteHover.setColor( highlight && !hovered ? highlightColor : defaultColor );
}
