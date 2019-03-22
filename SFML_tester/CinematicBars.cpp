#include "CinematicBars.h"

void CinematicBars::render(sf::RenderWindow & window)
{
	if (shouldDisplay)
	{
		window.draw(topSprite);
		window.draw(bottomSprite);
	}
}

void CinematicBars::update(float delta_t)
{

}

void CinematicBars::cleanup()
{
}

CinematicBars::CinematicBars()
{
	blackImage.create(CONFIG->defaultWidth, CONFIG->defaultHeight / 2.f, sf::Color(0, 0, 0, 255));
	blackTex.create(CONFIG->defaultWidth, CONFIG->defaultHeight / 2.f);
	blackTex.update(blackImage);

	topSprite.setTexture(blackTex);
	bottomSprite.setTexture(blackTex);

	topSprite.setOrigin(0, CONFIG->defaultHeight / 2.f);
	topSprite.setPosition(0, 0);

	bottomSprite.setOrigin(0, 0);
	bottomSprite.setPosition(0, CONFIG->defaultHeight);
}

CinematicBars::CinematicBars(ifstream & savefile)
{
	savefile.read(reinterpret_cast<char*> (&shouldDisplay), sizeof(shouldDisplay));
	savefile.read(reinterpret_cast<char*> (&pos), sizeof(pos));

	sf::Image blackImage;
	blackImage.create(CONFIG->defaultWidth, CONFIG->defaultHeight / 2.f, sf::Color(0, 0, 0, 255));
	blackTex.create(CONFIG->defaultWidth, CONFIG->defaultHeight / 2.f);
	blackTex.update(blackImage);

	topSprite.setTexture(blackTex);
	bottomSprite.setTexture(blackTex);

	topSprite.setOrigin(0, CONFIG->defaultHeight / 2.f);
	topSprite.setPosition(0, 0);

	bottomSprite.setOrigin(0, 0);
	bottomSprite.setPosition(0, CONFIG->defaultHeight);

	resetPosition();
}

CinematicBars::~CinematicBars()
{
	cleanup();
}

void CinematicBars::serialize(ofstream & savefile) const
{
	savefile.write(reinterpret_cast<const char*> (&shouldDisplay), sizeof(shouldDisplay));
	savefile.write(reinterpret_cast<const char*> (&pos), sizeof(pos));
}

void CinematicBars::setHeight(float y)
{
	pos = y;
	resetPosition();
}

void CinematicBars::addHeight(float y)
{
	pos += y;
	resetPosition();
}

float CinematicBars::getHeight() const
{
	return pos;
}

void CinematicBars::setDisplay(bool display)
{
	shouldDisplay = display;
}

bool CinematicBars::isDisplaying() const
{
	return shouldDisplay;
}

void CinematicBars::resetPosition()
{
	topSprite.setPosition(0, pos);
	bottomSprite.setPosition(0, CONFIG->defaultHeight - pos);
}
