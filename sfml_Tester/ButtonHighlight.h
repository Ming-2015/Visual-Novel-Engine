#pragma once
#include "MenuButton.h"
#include "Global.h"

class ButtonHighlight
{
public:
	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);								// all non-rendering game logics/mechanics should be implemented here
	void init();
	void cleanup();

	ButtonHighlight();
	~ButtonHighlight();

	bool autoHidden, skipHidden;
	bool skipButtonClicked;
	bool autoButtonClicked;

private:

	sf::Texture autoTexture;
	sf::Sprite autoSprite;
	sf::Texture skipTexture;
	sf::Sprite skipSprite;



	MenuButton * autoButton = nullptr;
	MenuButton * skipButton = nullptr;

	vector<MenuButton *> buttons;
	vector<sf::Sprite> sprites;

	int AUTO_BUTTON = 0;
	int SKIP_BUTTON = 1;
};

