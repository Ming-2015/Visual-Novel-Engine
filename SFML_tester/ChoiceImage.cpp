#include "ChoiceImage.h"

const float ChoiceImage::TOP_Y = -50;
const float ChoiceImage::BOTTOM_Y = 750;

void ChoiceImage::initText()
{
	choiceTextOutlineColor = sf::Color(0, 0, 0);
	choiceTextColor = sf::Color(255, 255, 255);
	choiceFont = sf::Font();
	if (!choiceFont.loadFromFile(GLOBAL->DisplayTextFont))
	{
		LOGGER->Log("ChoiceImage", "Unable to load default font");
	}

	choiceText.setFont(choiceFont);
	choiceText.setStyle(sf::Text::Regular);
	choiceText.setFillColor(choiceTextColor);
	choiceText.setOutlineColor(choiceTextOutlineColor);
	choiceText.setOutlineThickness(3);
	choiceText.setCharacterSize(32);

	choiceText.setString(choice);
	choiceText.setPosition( CONFIG->getWindowWidth() / 2.0f - choiceText.getLocalBounds().width / 2.0f, 
		yPos - sprite.getLocalBounds().height / 2.0f + 80.f );

}

ChoiceImage::ChoiceImage(const std::string & str, const std::string & flag, int numChoices, int index)
	: ItemImage("assets", "choicebox.png", 0, 0), 
	choice(str), flag(flag), numChoices(numChoices), index(index)
{
	xPos = CONFIG->getWindowWidth() / 2.0f ;
	yPos = TOP_Y + (BOTTOM_Y - TOP_Y) / float(numChoices + 1) * float(index + 1);

	sprite.setPosition(xPos, yPos);
	
	setChoiceboxColor(sf::Color(255, 0, 180));
	setAlpha(190.f);

	initText();
}

std::string ChoiceImage::getFlag() const
{
	return flag;
}

std::string ChoiceImage::getText() const
{
	return choice;
}

void ChoiceImage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	ItemImage::draw(target, states);
	target.draw(choiceText, states);
}

void ChoiceImage::setAlpha(float alpha)
{
	ItemImage::setAlpha(alpha);

	choiceTextColor.a = (sf::Uint8) alpha;
	choiceTextOutlineColor.a = (sf::Uint8) alpha;
	choiceText.setFillColor(choiceTextColor);
	choiceText.setOutlineColor(choiceTextOutlineColor);

}

void ChoiceImage::addAlpha(float alpha)
{
	ItemImage::addAlpha(alpha);

	choiceTextColor.a += (sf::Uint8) alpha;
	choiceTextOutlineColor.a += (sf::Uint8) alpha;
	choiceText.setFillColor(choiceTextColor);
	choiceText.setOutlineColor(choiceTextOutlineColor);

}

void ChoiceImage::setChoiceboxColor(sf::Color color)
{
	this->color = color;
	this->color.a = alpha;
	sprite.setColor(color);
}

