#pragma once

#include <SFML/Graphics.hpp>
#include "Effect.hpp"
#include "Global.h"
#include "Utility.h"

class LogBar : public Effect
{
public:
	LogBar();
	~LogBar();

	void setName(const std::string& name);
	void setLine(const std::string& line);

	void setPosition(int x, int y);

	bool isClicked(bool reset = false);
	bool isPressed() const;
	bool isOnHover() const;

protected:

	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

	// this will be centered
	const int nameTextPosX = 150, nameTextPosY = 112;
	sf::Text nameText;

	// this will be top-left aligned
	const int lineTextPosX = 450, lineTextPosY = 30;
	sf::Text lineText;
	sf::Font font;

	sf::Sprite bgSprite;
	sf::Texture bgUnselectedTex;
	sf::Texture bgSelectedTex;

	bool hovered = false;
	bool clicked = false;
	bool pressed = false;
};