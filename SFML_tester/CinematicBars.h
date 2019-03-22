#pragma once
#include "Global.h"
#include "Config.h"

class CinematicBars
{
public:
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);								// all non-rendering game logics/mechanics should be implemented here
	void cleanup();

	CinematicBars();
	CinematicBars(ifstream& savefile);
	~CinematicBars();

	void serialize(ofstream& savefile) const;
	void setHeight(float y);
	void addHeight(float y);
	float getHeight() const;
	
	void setDisplay(bool display);
	bool isDisplaying() const;

private:

	sf::Image blackImage;
	sf::Texture blackTex;
	sf::Sprite topSprite;
	sf::Sprite bottomSprite;

	bool shouldDisplay = false;
	float pos = 0;

	void resetPosition();
};

