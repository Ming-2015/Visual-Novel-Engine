#pragma once
#include "Button.h"
#include "Global.h"

class RadioButton : public Button
{
public:
	RadioButton(std::string str, int xPos, int yPos) 
		: textureUnselected(texture), buttonString(str), Button(GLOBAL->AssetRoot + "radioButtonUnselected.png", "", "", xPos, yPos)
	{
	}

	void setSelected(bool selected);
	bool getSelected() const;
	void setPosition(int xPos, int yPos);
	void setOtherRadioButtons(std::vector<RadioButton*> others) {
		otherRadioButtons = others;
	}

protected:
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

private:

	sf::Text buttonText;
	sf::Font buttonFont;
	std::string buttonString;
	const int xPosOffset = 32, yPosOffset = 0;

	sf::Texture textureSelected;
	sf::Texture& textureUnselected;
	bool selected = false;
	std::vector<RadioButton*> otherRadioButtons;
};