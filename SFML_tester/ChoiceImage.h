#pragma once
#include "ItemImage.h"
#include "Config.h"

class ChoiceImage : public ItemImage
{
public:
	ChoiceImage(const std::string& str, const std::string& flag, int numChoices, int index);

	std::string getFlag() const;
	std::string getText() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setChoiceboxColor(sf::Color color);

	void setAlpha(float alpha);
	void addAlpha(float alpha);

private:
	void initText();

	sf::Text choiceText;
	sf::Font choiceFont;
	sf::Color choiceTextColor;
	sf::Color choiceTextOutlineColor;

	std::string choice;
	std::string flag;
	int numChoices, index;

	const static float TOP_Y;
	const static float BOTTOM_Y;
};