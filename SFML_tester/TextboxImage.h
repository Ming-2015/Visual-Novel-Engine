#pragma once
#include "ItemImage.h"

class TextboxImage : public ItemImage
{
public:
	TextboxImage();

	void setText(const std::string& s);
	void setName(const std::string& s);
	void setDisplay(const std::string& s_name, const std::string& s_dialogue);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setTextboxColor(sf::Color color);

private:
	void initText();

	sf::Text dialogueText;
	sf::Font dialogueFont;
	sf::Color dialogueTextColor;
	std::string dialogue;

	sf::Text nameText;
	sf::Font nameFont;
	sf::Color nameTextColor;
	std::string name;
};