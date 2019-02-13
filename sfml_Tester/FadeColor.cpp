#include "FadeColor.h"
#include "Config.h"

FadeColor::FadeColor(string fadeColor, int alphaIncrement, float fadeSpeed, int R, int G, int B)
	: Effect("FadeColor"), alphaIncrement(alphaIncrement), fadeSpeed(fadeSpeed), R(R), G(G), B(B)
{
	if (fadeColor == "white") 
	{
		currentColorR = 255;
		currentColorG = 255;
		currentColorB = 255;
		LOGGER->Log("Fade", "Fade Color: White");
	}
	else if (fadeColor == "black")
	{
		currentColorR = 0;
		currentColorG = 0;
		currentColorB = 0;
		LOGGER->Log("Fade", "Fade Color: Black");
	}
	else if (fadeColor == "other") 
	{
		currentColorR = R;
		currentColorG = G;
		currentColorB = B;
		LOGGER->Log("Fade", "Fade Color: Specified");
	}
	else 
	{
		currentColorR = 0;
		currentColorG = 0;
		currentColorB = 0;
		LOGGER->Log("Fade", "Fade Color: Defaulted to Black");
	}
}

FadeColor::~FadeColor()
{

}

bool FadeColor::onLoad()
{
	rectangle.setSize(sf::Vector2f(CONFIG->getWindowWidth(), CONFIG->getWindowHeight()));
	rectangle.setFillColor(sf::Color::Color(currentColorR, currentColorG, currentColorB, currentAlpha));
	rectangle.setPosition(0, 0);

	return true;
}

void FadeColor::onUpdate(float delta_t)
{
	if (startFade)
	{
		if (clock.getElapsedTime().asMilliseconds() > fadeSpeed) {
			if (currentAlpha < endAlpha) {
				currentAlpha += alphaIncrement;
				clock.restart();
				if (currentAlpha > endAlpha) currentAlpha = endAlpha;
			}
			else
			{
				doneFade = true;
			}
		}
		rectangle.setFillColor(sf::Color(0, 0, 0, currentAlpha));
	}
}

void FadeColor::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (startFade)
	{
		target.draw(rectangle);
	}
}

void FadeColor::onHandleInput(sf::Event &e, sf::RenderWindow & window)
{

}

void FadeColor::start()
{
	clock.restart();
	currentAlpha = 0;
	startFade = true;
}

bool FadeColor::isDone(bool reset)
{
	bool temp = doneFade;
	if (reset) temp = false;
	return temp;
}

void FadeColor::skip()
{
	currentAlpha = 255;
	doneFade = true;
}