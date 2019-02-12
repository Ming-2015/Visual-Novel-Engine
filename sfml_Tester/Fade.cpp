#include "Fade.h"
#include "Config.h"

Fade::Fade(string filename, string fadeColor, int alphaIncrement, float fadeSpeed) 
	: Effect("Fade"), alphaIncrement(alphaIncrement), fadeSpeed(fadeSpeed), filename(filename)
{
	//If user specifies that there is to be a BWFade
	if (fadeColor == "white") {
		currentColorR = 255;
		currentColorG = 255;
		currentColorB = 255;
		isBWFade = true;
	}
	else if (fadeColor == "black") {
		currentColorR = 0;
		currentColorG = 0;
		currentColorB = 0;
		isBWFade = true;
	}
	else     //If fadeColor != white || != black
	{
		isBWFade = false;
		LOGGER->Log("Fade", "No BW Fade");
	}

	currentAlpha = 0;
	endAlpha = 255;
}

Fade::~Fade()
{

}

bool Fade::onLoad()
{
	rectangle.setSize(sf::Vector2f(CONFIG->getWindowWidth(), CONFIG->getWindowHeight()));
	rectangle.setFillColor(sf::Color::Color(currentColorR, currentColorG, currentColorB, currentAlpha));
	rectangle.setPosition(0, 0);


	//Load picture to sprite, done regardless of BWFade
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

		if (isBWFade) 
		{
			rectangle.setFillColor(sf::Color(0, 0, 0, currentAlpha));
		}

		//Fade in next photo DONE REGARDLESS OF BWFADE
		picInSprite.setColor(sf::Color(255, 255, 255, currentAlpha));
		
	}
}

void Fade::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (startFade)
	{
		if (isBWFade)
		{
			target.draw(rectangle);
		}
		else
		{
			target.draw(picInSprite);
		}
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