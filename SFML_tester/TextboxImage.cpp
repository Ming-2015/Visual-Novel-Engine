#include "TextboxImage.h"

TextboxImage::TextboxImage()
	: ItemImage("textbox", "textboxw.png", 0, 0)
{
	initText();
}

void TextboxImage::setText(const std::string & s)
{
	dialogue = s;
	dialogueText.setString(dialogue);
}

void TextboxImage::setName(const std::string & s)
{
	name = s;
	nameText.setString(name);
}

void TextboxImage::setDisplay(const std::string & s_name, const std::string & s_dialogue)
{
	setText(s_dialogue);
	setName(s_name);
}

void TextboxImage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	ItemImage::draw(target, states);
	target.draw(dialogueText, states);
	target.draw(nameText, states);
}

void TextboxImage::setTextboxColor(sf::Color color)
{
	this->color = color;
	sprite.setColor(color);
}

void TextboxImage::setAlpha(float alpha)
{
	ItemImage::setAlpha(alpha);

	dialogueTextColor.a = (sf::Uint8) alpha;
	dialogueTextOutlineColor.a = (sf::Uint8) alpha;
	dialogueText.setFillColor(dialogueTextColor);
	dialogueText.setOutlineColor(dialogueTextOutlineColor);

	nameTextColor.a = (sf::Uint8) alpha;
	nameTextOutlineColor.a = (sf::Uint8) alpha;
	nameText.setFillColor(nameTextColor);
	nameText.setOutlineColor(nameTextOutlineColor);
}

void TextboxImage::addAlpha(float alpha)
{
	ItemImage::addAlpha(alpha);

	dialogueTextColor.a += (sf::Uint8) alpha;
	dialogueTextOutlineColor.a += (sf::Uint8) alpha;
	dialogueText.setFillColor(dialogueTextColor);
	dialogueText.setOutlineColor(dialogueTextOutlineColor);

	nameTextColor.a += (sf::Uint8) alpha;
	nameTextOutlineColor.a += (sf::Uint8) alpha;
	nameText.setFillColor(nameTextColor);
	nameText.setOutlineColor(nameTextOutlineColor);
}

void TextboxImage::initText()
{
	dialogueTextOutlineColor = sf::Color(0, 0, 0);
	dialogueTextColor = sf::Color(255, 255, 255);
	dialogueFont = sf::Font();
	if (!dialogueFont.loadFromFile(GLOBAL->DisplayTextFont))
	{
		LOGGER->Log("TextboxImage", "Unable to load default font");
	}

	dialogueText.setFont(dialogueFont);
	dialogueText.setStyle(sf::Text::Regular);
	dialogueText.setFillColor(dialogueTextColor);
	dialogueText.setOutlineColor(dialogueTextOutlineColor);
	dialogueText.setOutlineThickness(3);
	dialogueText.setCharacterSize(32);
	dialogueText.setPosition(230.0f, 705.0f);

	nameTextOutlineColor = sf::Color(0, 0, 0);
	nameTextColor = sf::Color(255, 255, 255);
	nameFont = sf::Font();
	if (!nameFont.loadFromFile(GLOBAL->DisplayNameFont))
	{
		LOGGER->Log("TextboxImage", "Unable to load name font");
	}

	nameText.setFont(nameFont);
	nameText.setStyle(sf::Text::Bold);
	nameText.setFillColor(nameTextColor);
	nameText.setOutlineColor(nameTextOutlineColor);
	nameText.setOutlineThickness(2);
	nameText.setCharacterSize(32);
	nameText.setPosition(230.0f, 643.0f);
}

