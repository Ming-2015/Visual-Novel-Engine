#include "RadioButton.h"

void RadioButton::setSelected(bool selected)
{
	this->selected = selected;
	if (selected)
	{
		sprite.setTexture(textureSelected);
		for (RadioButton * button : otherRadioButtons)
		{
			if (button != this)
			{
				button->setSelected(false);
			}
		}
	}
	else
	{
		sprite.setTexture(textureUnselected);
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
	sprite.setPosition(xPos, yPos);
	buttonText.setPosition(xPos + xPosOffset, yPos + yPosOffset);
}

bool RadioButton::onLoad()
{
	bool ret = Button::onLoad();
	if (!textureSelected.loadFromFile(GLOBAL->AssetRoot + "radioButtonSelected.png"))
	{
		LOGGER->Log("RadioButton", "Unable to load unselected radio button texture");
		ret = false;
	}
	textureSelected.setSmooth(true);

	if (!buttonFont.loadFromFile(GLOBAL->UserInterfaceFont))
	{
		LOGGER->Log("RadioButton", "Unable to load User Interface Font");
		ret = false;
	}

	buttonText.setFont(buttonFont);
	buttonText.setStyle(sf::Text::Regular);
	buttonText.setOutlineThickness(2);
	buttonText.setFillColor(sf::Color(255, 255, 255, 255));
	buttonText.setOutlineColor(sf::Color(0, 0, 0, 255));
	buttonText.setCharacterSize(20);
	buttonText.setString(buttonString);
	buttonText.setPosition(xPos + xPosOffset, yPos + yPosOffset);

	return ret;
}

void RadioButton::onUpdate(float delta_t)
{
	//bool shouldUnselect = false;
	//for (int i = 0; i < otherRadioButtons.size(); i++)
	//{
	//	if (otherRadioButtons[i] == this || !otherRadioButtons[i])
	//	{
	//		continue;
	//	}

	//	if (otherRadioButtons[i]->getSelected())
	//	{
	//		shouldUnselect = true;
	//		break;
	//	}
	//}
	//if (shouldUnselect)
	//{
	//	setSelected(false);
	//}
}

void RadioButton::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	Button::onDraw(target, states);

	target.draw(buttonText, states);
}

void RadioButton::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
	switch (e.type)
	{
		case sf::Event::MouseMoved:
		{
			if (sprite.getGlobalBounds().contains(mousePosF) 
				|| buttonText.getGlobalBounds().contains(mousePosF))
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
				if (sprite.getGlobalBounds().contains(mousePosF)
					|| buttonText.getGlobalBounds().contains(mousePosF)) 
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
					setSelected(true);
				}
			}
			break;
		}
	}
}
