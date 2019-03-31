#pragma once
#include "Effect.hpp"
#include "Global.h"
#include "Config.h"
#include "ResourceManager.h"

class RadioButton : public Effect
{
public:
	RadioButton(std::string path, int xPos, int yPos) 
		: textureTextPath(path), xPos(xPos), yPos(yPos), Effect("RadioButton")
	{
	}

	void addOffset(float xOffset, float yOffset) 
	{ 
		xPosOffset += xOffset; 
		yPosOffset += yOffset; 
		setPosition(xPos, yPos); 
	}
	void setSelected(bool selected);
	bool getSelected() const;
	void setPosition(int xPos, int yPos);
	bool getHovered() const { return hovered; }

	void setOtherRadioButtons(std::vector<RadioButton*> others) 
	{
		otherRadioButtons = others;
	}

protected:
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

private:

	int xPosOffset = 0, yPosOffset = 0;
	float xPos, yPos;

	sf::Sprite radioButton;
	sf::Texture* textureSelectedUnhovered;
	sf::Texture* textureUnselectedUnhovered;
	sf::Texture* textureSelectedHovered;
	sf::Texture* textureUnselectedHovered;

	sf::Sprite radioText;
	sf::Texture* textureText;
	std::string textureTextPath;

	bool selected = false;
	bool hovered = false;
	bool pressed = false;

	std::vector<RadioButton*> otherRadioButtons;

	const sf::Texture& getButtonTexture(bool selected, bool hovered);
	void setRadioButtonTexture();
};