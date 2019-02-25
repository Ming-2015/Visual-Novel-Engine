#pragma once
#include "MainButton.h"
#include "Global.h"

class ConfirmationPrompt
{
public:
	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);								// all non-rendering game logics/mechanics should be implemented here
	void init();
	void cleanup();

	ConfirmationPrompt(std::string prompt, std::string title);
	~ConfirmationPrompt();

	bool shouldBeHidden() const;
	void hidePrompt(bool hide);

	bool isNoButtonClicked(bool reset = false);
	bool isYesButtonClicked(bool reset = false);

private:

	sf::Texture boxTexture;
	sf::Sprite boxSprite;

	MainButton * yesButton = nullptr;
	MainButton * noButton = nullptr;

	vector<MainButton*> buttons;

	std::string prompt;
	std::string title;

	sf::Font font;
	sf::Text text;

	bool isHidden;
	bool noButtonClicked;
	bool yesButtonClicked;
};
