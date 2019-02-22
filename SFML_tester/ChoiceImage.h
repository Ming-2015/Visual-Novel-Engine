#pragma once
#include "ItemImage.h"
#include "Config.h"

class ChoiceImage : public ItemImage
{
public:
	ChoiceImage(const std::string& str, const std::string& flag, int numChoices, int index);
	ChoiceImage(ifstream& file);

	void serialize(ofstream& savefile);

	std::vector<std::string> getFlags() const;
	std::string getText() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setChoiceboxColor(sf::Color color);
	void setAlpha(float alpha);
	void addAlpha(float alpha);

	void setPosition(float x, float y);
	void move(float x, float y);

private:
	void initText();

	sf::Text choiceText;
	sf::Font choiceFont;
	sf::Color choiceTextColor;
	sf::Color choiceTextOutlineColor;

	std::string choice;
	std::vector<std::string> flags;
	int numChoices, index;

	const static float TOP_Y;
	const static float BOTTOM_Y;

	sf::Texture choiceboxBgTex;
	sf::Sprite choiceboxBg;
};