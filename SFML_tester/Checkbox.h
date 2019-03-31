#pragma once
#include "Effect.hpp"
#include "Global.h"
#include "Config.h"
#include "ResourceManager.h"

class CheckBox : public Effect
{
public:
	CheckBox(std::string path, int xPos, int yPos)
		: textureTextPath(path), xPos(xPos), yPos(yPos), Effect("Checkbox")
	{
	}

	void setSelected(bool selected);
	bool getSelected() const;
	void setPosition(int xPos, int yPos);
	bool getHovered() const { return hovered; }

protected:
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

private:

	int xPosOffset = 0, yPosOffset = 0;
	float xPos, yPos;

	sf::Sprite checkBox;
	sf::Texture* textureSelectedUnhovered;
	sf::Texture* textureUnselectedUnhovered;
	sf::Texture* textureSelectedHovered;
	sf::Texture* textureUnselectedHovered;

	sf::Sprite checkBoxText;
	sf::Texture* textureText;
	std::string textureTextPath;

	bool selected = false;
	bool hovered = false;
	bool pressed = false;

	const sf::Texture& getButtonTexture(bool selected, bool hovered);
	void setCheckBoxTexture();
};