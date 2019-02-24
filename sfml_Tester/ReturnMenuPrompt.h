#pragma once
#include "MainButton.h"
#include "Global.h"

class ReturnMenuPrompt
{
public:
	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);								// all non-rendering game logics/mechanics should be implemented here
	void init();
	void cleanup();

	ReturnMenuPrompt();
	~ReturnMenuPrompt();

	bool isHidden;
	bool noButtonClicked;
	bool yesButtonClicked;

private:

	sf::Texture boxTexture;
	sf::Sprite boxSprite;

	MainButton * yesButton = nullptr;
	MainButton * noButton = nullptr;

	vector<MainButton*> buttons;

};
