#include "RadioButton.h"

void RadioButton::setSelected(bool selected)
{
	this->selected = selected;
	setRadioButtonTexture();

	if (selected)
	{
		for (RadioButton* button : otherRadioButtons)
		{
			if (button != this)
			{
				button->setSelected(false);
			}
		}
	}
}

bool RadioButton::getSelected() const
{
	return selected;
}

void RadioButton::setPosition(int xPos, int yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
	radioButton.setPosition(xPos, yPos + - 4 + yPosOffset);
	radioText.setPosition(xPos + xPosOffset, yPos + yPosOffset);
}

bool RadioButton::onLoad()
{
	bool ret = true;

	// initialize the textures
	textureSelectedUnhovered = RESOURCE->getTexture(GLOBAL->AssetRoot + "RBSelected.png");
	textureSelectedHovered = RESOURCE->getTexture(GLOBAL->AssetRoot + "RBSelectedHover.png");
	textureUnselectedUnhovered = RESOURCE->getTexture(GLOBAL->AssetRoot + "RBUnselected.png");
	textureUnselectedHovered = RESOURCE->getTexture(GLOBAL->AssetRoot + "RBUnselectedHover.png");

	// initialize the text
	textureText = RESOURCE->getTexture(textureTextPath);

	// wait for everything to load
	RESOURCE->joinAll();

	// initialize radiobutton
	setRadioButtonTexture();

	// initialize text
	radioText.setTexture(*textureText);	
	radioText.setTextureRect(sf::IntRect(0, 0, textureText->getSize().x, textureText->getSize().y));

	// set the initial positions
	xPosOffset = radioButton.getLocalBounds().width;
	yPosOffset = radioButton.getLocalBounds().height - radioText.getLocalBounds().height;
	yPosOffset /= 2.f;
	setPosition(xPos, yPos);

	return ret;
}

void RadioButton::onUpdate(float delta_t)
{
	setRadioButtonTexture();
}

void RadioButton::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(radioButton, states);
	target.draw(radioText, states);
}

void RadioButton::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
	switch (e.type)
	{
		case sf::Event::MouseMoved:
		{
			if (radioButton.getGlobalBounds().contains(mousePosF) 
				|| radioText.getGlobalBounds().contains(mousePosF))
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
				if (radioButton.getGlobalBounds().contains(mousePosF)
					|| radioText.getGlobalBounds().contains(mousePosF)) 
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
					setSelected(true);
				}
			}
			break;
		}
	}
}

const sf::Texture & RadioButton::getButtonTexture(bool selected, bool hovered)
{
	if (!selected && !hovered)
	{
		return *textureUnselectedUnhovered;
	}
	else if (selected && !hovered)
	{
		return *textureSelectedUnhovered;
	}
	else if (!selected && hovered)
	{
		return *textureUnselectedHovered;
	}
	else
	{
		return *textureSelectedHovered;
	}
}

void RadioButton::setRadioButtonTexture()
{
	const sf::Texture& radioTex = getButtonTexture(selected, hovered);
	radioButton.setTexture(radioTex);
	radioButton.setTextureRect(sf::IntRect(0, 0, radioTex.getSize().x, radioTex.getSize().y));
}
