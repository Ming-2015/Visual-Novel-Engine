#pragma once
#include "Effect.hpp"
#include "ResourceManager.h"

class DropboxButton : public Effect {
public:
	DropboxButton(std::string texPathUnhovered, std::string texPathHovered, int xPos, int yPos)
		: xPos(xPos), yPos(yPos), pathTexHovered(texPathHovered), pathTexUnhovered(texPathUnhovered), Effect("DropboxButton")
	{
	}

	void setOffset(float xOffset, float yOffset)
	{
		xPosOffset = xOffset;
		yPosOffset = yOffset;
		setPosition(xPos, yPos);
	}

	void setText(std::string str);
	void setFont(std::string fontPath);

	std::string getFont() const { return buttonFontStr; }
	std::string getText() const { return buttonTextStr; }

	void resetClicked();
	bool isClicked(bool reset);
	void setPosition(int xPos, int yPos);
	void move(int xOffset, int yOffset);
	bool getHovered() const { return hovered; }

	int getButtonWidth() const;
	int getButtonHeight() const;

protected:
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

private:

	int xPosOffset = 0, yPosOffset = 0;
	float xPos, yPos;

	sf::Sprite buttonSprite;

	std::string pathTexHovered;
	sf::Texture* texHovered;
	std::string pathTexUnhovered;
	sf::Texture* texUnhovered;

	std::string buttonTextStr;
	sf::Text buttonText;
	sf::Font* buttonFont;
	std::string buttonFontStr;

	bool clicked = false;
	bool hovered = false;
	bool pressed = false;

	const sf::Texture& getButtonTexture(bool hovered);
	void setButtonTexture();
};