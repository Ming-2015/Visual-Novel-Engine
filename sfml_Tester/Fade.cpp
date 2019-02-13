#include "Fade.h"
#include "Config.h"

Fade::Fade(string filename, int alphaIncrement, float fadeSpeed) 
	: Effect("Fade"), alphaIncrement(alphaIncrement), fadeSpeed(fadeSpeed), filename(filename)
{
	currentAlpha = 0;
	endAlpha = 255;
}

Fade::~Fade()
{

}

bool Fade::onLoad()
{
	if (!picInTexture.loadFromFile(filename))
		LOGGER->Log("Fade", "Fade Unsuccessful. Picture not found.");
	picInSprite.setTexture(picInTexture);

	return true;
}

void Fade::onUpdate(float delta_t)
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
		picInSprite.setColor(sf::Color(255, 255, 255, currentAlpha));
	}
}

void Fade::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (startFade)
	{
			target.draw(picInSprite);
	}
}

void Fade::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{

}

void Fade::start() 
{
	clock.restart();
	currentAlpha = 0;
	startFade = true;
}

bool Fade::isDone(bool reset) 
{
	bool temp = doneFade;
	if (reset) temp = false;
	return temp;
}

void Fade::skip() 
{
	currentAlpha = 255;
	doneFade = true;
}