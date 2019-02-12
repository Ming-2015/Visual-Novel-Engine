#pragma once
#include "Effect.hpp"
#include <string>
#include "Config.h"

using namespace std;

class Fade : public Effect
{
public:

	void start();
	bool isDone();
	void skip();
	Fade(string filename, string fadeColor);
	~Fade();
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

private:
	sf::Clock clock;
	
	sf::Texture picInTexture;
	sf::Sprite picInSprite;

	sf::RectangleShape rectangle;
	int currentColorR;
	int currentColorG;
	int currentColorB;
	bool isBWFade;

	int currentAlpha;
	int endAlpha;
	int alphaIncrement;
	float fadeSpeed;
};