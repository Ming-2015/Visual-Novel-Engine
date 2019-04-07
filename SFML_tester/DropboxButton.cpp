#include "DropboxButton.h"

const sf::Texture & DropboxButton::getButtonTexture(bool hovered)
{
	if (hovered)
	{
		return *texHovered;
	}
	else
	{
		return *texUnhovered;
	}
}

void DropboxButton::setButtonTexture()
{
	const sf::Texture& buttonTex = getButtonTexture(hovered);
	buttonSprite.setTexture(buttonTex);
	buttonSprite.setTextureRect(sf::IntRect(0, 0, buttonTex.getSize().x, buttonTex.getSize().y));
}

void DropboxButton::setText(std::string str)
{
	buttonTextStr = str;
	buttonText.setString(str);
}

void DropboxButton::setFont(std::string fontPath)
{
	buttonFontStr = fontPath;
	buttonFont = RESOURCE->getFont(fontPath, true);
	buttonText.setFont(*buttonFont);
}

void DropboxButton::resetClicked()
{
	clicked = false;
}

bool DropboxButton::isClicked(bool reset)
{
	bool temp = clicked;
	if (reset) clicked = false;
	return temp;
}

void DropboxButton::setPosition(int xPos, int yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
	buttonSprite.setPosition(xPos, yPos);
	buttonText.setPosition(xPos + xPosOffset, yPos + yPosOffset);
}

void DropboxButton::move(int xOffset, int yOffset)
{
	this->xPos += xOffset;
	this->yPos += yOffset;
	buttonSprite.setPosition(xPos, yPos);
	buttonText.setPosition(xPos + xPosOffset, yPos + yPosOffset);
}

int DropboxButton::getButtonWidth() const
{
	return buttonSprite.getLocalBounds().width;
}

int DropboxButton::getButtonHeight() const
{
	return buttonSprite.getLocalBounds().height;
}

bool DropboxButton::onLoad()
{
	texHovered = RESOURCE->getTexture(pathTexHovered);
	texUnhovered = RESOURCE->getTexture(pathTexUnhovered);
	
	RESOURCE->joinAll();
	setButtonTexture();

	buttonText.setCharacterSize(24);
	buttonText.setFillColor(sf::Color::White);
	buttonText.setStyle(sf::Text::Regular);

	xPosOffset = 5;
	yPosOffset = 40;
	setPosition(xPos, yPos);

	return true;
}

void DropboxButton::onUpdate(float delta_t)
{
	setButtonTexture();
}

void DropboxButton::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(buttonSprite, states);
	target.draw(buttonText, states);
}

void DropboxButton::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
	switch (e.type)
	{
	case sf::Event::MouseMoved:
	{
		if (buttonSprite.getGlobalBounds().contains(mousePosF))
		{
			hovered = true;
		}
		else
		{
			hovered = false;
		}
		break;
	}

	case sf::Event::MouseButtonPressed:
	{
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			if (buttonSprite.getGlobalBounds().contains(mousePosF))
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
		}
		break;
	}
	}
}
