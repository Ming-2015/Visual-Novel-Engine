#pragma once

#include <SFML/Graphics.hpp>
#include "Effect.hpp"
#include "Global.h"
#include "Config.h"

class LoadfileImage : public Effect
{
public:
	LoadfileImage(int y);
	~LoadfileImage();

	void setImage(const sf::Image& image);
	void setTitle(const std::string& title);
	void setDate(const std::string& date);
	void useDefaultSprite();

	void setPosition(int x, int y);

	bool isClicked(bool reset = false);
	bool isPressed() const;
	bool isOnHover() const;

	int x;

	void resetDefault();

protected:

	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

	sf::Texture saveTex;
	std::string saveTitle;
	std::string saveDate;

	sf::Sprite saveSprite;
	sf::Text saveTitleText;
	sf::Text saveDateText;
	sf::Font font;

	sf::Sprite bgSprite;
	sf::Texture bgUnselectedTex;

	sf::Sprite bgBoxSprite;
	sf::Texture bgBoxUnselectedTex;
	sf::Texture bgBoxSelectedTex;

	sf::Sprite noDataSprite;
	sf::Texture noDataTex;

	bool hovered = false;
	bool clicked = false;
	bool pressed = false;

	const unsigned int stringLength = 28;
	const unsigned int borderSize = 15;

	void setToScale();
	
};