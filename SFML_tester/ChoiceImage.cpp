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

	choiceText.setOrigin(choiceText.getLocalBounds().width / 2.0f, 
		choiceText.getLocalBounds().height / 2.0f);

	choiceText.setPosition(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width / 2.0f,
		sprite.getGlobalBounds().top + sprite.getGlobalBounds().height / 2.0f );

}

ChoiceImage::ChoiceImage(const std::string & str, const std::string & flag, int numChoices, int index)
	: ItemImage("assets", "choicebox_fg.png", 0, 0), 
	choice(str), numChoices(numChoices), index(index)
{
	xPos = CONFIG->getWindowWidth() / 2.0f ;
	yPos = TOP_Y + (BOTTOM_Y - TOP_Y) / float(numChoices + 1) * float(index + 1);

	sprite.setPosition(xPos, yPos);

	if (!choiceboxBgTex.loadFromFile(GLOBAL->ImageRoot + "assets/choicebox_bg.png"))
	{
		LOGGER->Log("ChoiceImage", "Unable to log choice background image");
	}
	choiceboxBg.setTexture(choiceboxBgTex);
	choiceboxBg.setPosition(xPos, yPos);
	choiceboxBg.setOrigin(choiceboxBg.getLocalBounds().width / 2.0f, choiceboxBg.getLocalBounds().height / 2.0f);
	
	setChoiceboxColor(sf::Color(255, 0, 180));
	setAlpha(190.f);

	initText();

	flags = UTILITY->split(flag, ',');
	flags = UTILITY->trim(flags);
}

std::vector<std::string> ChoiceImage::getFlags() const
{
	return flags;
}

std::string ChoiceImage::getText() const
{
	return choice;
}

void ChoiceImage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(choiceboxBg, states);
	ItemImage::draw(target, states);
	target.draw(choiceText, states);
}

void ChoiceImage::setAlpha(float alpha)
{
	ItemImage::setAlpha(alpha);

	choiceboxBg.setColor(color);

	choiceTextColor.a = (sf::Uint8) alpha;
	choiceTextOutlineColor.a = (sf::Uint8) alpha;
	choiceText.setFillColor(choiceTextColor);
	choiceText.setOutlineColor(choiceTextOutlineColor);

}

void ChoiceImage::addAlpha(float alpha)
{
	ItemImage::addAlpha(alpha);

	choiceboxBg.setColor(color);

	choiceTextColor.a += (sf::Uint8) alpha;
	choiceTextOutlineColor.a += (sf::Uint8) alpha;
	choiceText.setFillColor(choiceTextColor);
	choiceText.setOutlineColor(choiceTextOutlineColor);

}

void ChoiceImage::setPosition(float x, float y)
{
	ItemImage::setPosition(x, y);

	choiceboxBg.setPosition(xPos, yPos);
}

void ChoiceImage::move(float x, float y)
{
	ItemImage::move(x, y);

	choiceboxBg.setPosition(xPos, yPos);
}

void ChoiceImage::setChoiceboxColor(sf::Color color)
{
	this->color = color;
	this->color.a = alpha;
	sprite.setColor(this->color);

	choiceboxBg.setColor(this->color);
}

