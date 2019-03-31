#include "CheckBox.h"

void CheckBox::setSelected(bool selected)
{
	this->selected = selected;
	setCheckBoxTexture();
}

bool CheckBox::getSelected() const
{
	return selected;
}

void CheckBox::setPosition(int xPos, int yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
	checkBox.setPosition(xPos, yPos);
	checkBoxText.setPosition(xPos + xPosOffset, yPos + yPosOffset);
}

bool CheckBox::onLoad()
{
	bool ret = true;

	// initialize the textures
	textureSelectedUnhovered = RESOURCE->getTexture(GLOBAL->AssetRoot + "CBSelected.png");
	textureSelectedHovered = RESOURCE->getTexture(GLOBAL->AssetRoot + "CBSelectedHover.png");
	textureUnselectedUnhovered = RESOURCE->getTexture(GLOBAL->AssetRoot + "CBUnselected.png");
	textureUnselectedHovered = RESOURCE->getTexture(GLOBAL->AssetRoot + "CBUnselectedHover.png");

	// initialize the text
	textureText = RESOURCE->getTexture(textureTextPath);

	// wait for everything to load
	RESOURCE->joinAll();

	// initialize CheckBox
	setCheckBoxTexture();

	// initialize text
	checkBoxText.setTexture(*textureText);
	checkBoxText.setTextureRect(sf::IntRect(0, 0, textureText->getSize().x, textureText->getSize().y));

	// set the initial positions
	xPosOffset = checkBox.getLocalBounds().width;
	yPosOffset = checkBox.getLocalBounds().height - checkBoxText.getLocalBounds().height;
	yPosOffset /= 2.f;
	setPosition(xPos, yPos);

	return ret;
}

void CheckBox::onUpdate(float delta_t)
{
	setCheckBoxTexture();
}

void CheckBox::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(checkBox, states);
	target.draw(checkBoxText, states);
}

void CheckBox::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
	switch (e.type)
	{
	case sf::Event::MouseMoved:
	{
		if (checkBox.getGlobalBounds().contains(mousePosF)
			|| checkBoxText.getGlobalBounds().contains(mousePosF))
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
			if (checkBox.getGlobalBounds().contains(mousePosF)
				|| checkBoxText.getGlobalBounds().contains(mousePosF))
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
				setSelected(!getSelected());
			}
		}
		break;
	}
	}
}

const sf::Texture & CheckBox::getButtonTexture(bool selected, bool hovered)
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

void CheckBox::setCheckBoxTexture()
{
	const sf::Texture& checkBoxTex = getButtonTexture(selected, hovered);
	checkBox.setTexture(checkBoxTex);
	checkBox.setTextureRect(sf::IntRect(0, 0, checkBoxTex.getSize().x, checkBoxTex.getSize().y));
}
