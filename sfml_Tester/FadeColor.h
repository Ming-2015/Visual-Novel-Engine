#pragma once
#include "Effect.hpp"
#include <string>
#include "Config.h"

using namespace std;

class FadeColor : public Effect
{
public:
	void start();
	bool isDone(bool reset = false);
	void skip();
	FadeColor(string fadeColor, int alphaIncrement = 10, float fadeSpeed = 20.0f, int R = 0, int G = 0, int B = 0);
	~FadeColor();
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

private:
	sf::Clock clock;

	sf::RectangleShape rectangle;
	int currentColorR;
	int currentColorG;
	int currentColorB;
	int R;
	int G;
	int B;

	int currentAlpha;
	int endAlpha;
	int alphaIncrement;
	float fadeSpeed;

	bool startFade = false;
	bool doneFade = false;
};