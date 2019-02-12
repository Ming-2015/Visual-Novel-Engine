#include "Fade.h"
#include "Config.h"

Fade::Fade(string filename, string fadeColor) : Effect("Fade")
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
		LOGGER->Log("Game State", "No Fade");
	}

	//Load picture to sprite, done regardless of BWFade
	if (!picInTexture.loadFromFile(filename))
		LOGGER->Log("Game State", "Fade Unsuccessful. Picture not found.");
	picInSprite.setTexture(picInTexture);
}

Fade::~Fade()
{

}

bool Fade::onLoad()
{
	currentAlpha = 0;
	alphaIncrement = 10;
	endAlpha = 255;
	fadeSpeed = 20.0f;
	rectangle.setSize(sf::Vector2f(CONFIG->getWindowWidth(), CONFIG->getWindowHeight()));
	rectangle.setFillColor(sf::Color::Color(currentColorR, currentColorG, currentColorB, currentAlpha));
	rectangle.setPosition(0, 0);
	return true;
}

void Fade::onUpdate(float delta_t)
{
	if (isBWFade) {
		if (clock.getElapsedTime().asMilliseconds() > fadeSpeed) {
			if (currentAlpha < endAlpha) {
				currentAlpha += alphaIncrement;
				rectangle.setFillColor(sf::Color::Color(0, 0, 0, currentAlpha));
			}
			else {
				//What to do after Fade Finish
			}
			clock.restart();
		}
	}

	//Fade in next photo DONE REGARDLESS OF BWFADE
	if (clock.getElapsedTime().asMilliseconds() > fadeSpeed) {
		if (currentAlpha > endAlpha) {
			currentAlpha += alphaIncrement;
			picInSprite.setColor(sf::Color::Color(255, 255, 255, currentAlpha));
		}
		else {
			//What to do after Fade in Finish
		}
		clock.restart();
	}
}

void Fade::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (isBWFade)
	{
		target.draw(rectangle);
	}

	target.draw(picInSprite);
}

void Fade::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{

}

void Fade::start() 
{

}

bool Fade::isDone() 
{
	return true;
}

void Fade::skip() 
{

}