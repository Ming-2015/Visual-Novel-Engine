#pragma once
#include "MainButton.h"
#include "Global.h"

class DrawMainButton
{
public:

	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);								// all non-rendering game logics/mechanics should be implemented here
	void init();
	void cleanup();

	void setAlpha(float a);
	void addAlpha(float offset);

	void highlightAutoButton(bool h);
	void highlightSkipButton(bool h);

	DrawMainButton();
	~DrawMainButton();

	bool configButtonClicked;
	bool exitButtonClicked;
	bool quickLoadButtonClicked;
	bool quickSaveButtonClicked;
	bool saveButtonClicked;
	bool loadButtonClicked;
	bool autoButtonClicked;
	bool skipButtonClicked;
	bool closeButtonClicked;
	bool logButtonClicked;

private:

	MainButton * configButton;
	MainButton * exitButton;
	MainButton * QLButton;
	MainButton * loadButton;
	MainButton * QSButton;
	MainButton * saveButton;
	MainButton * autoButton;
	MainButton * skipButton;
	MainButton * closeButton;
	MainButton * logButton;

	vector<MainButton*> buttons;

	int CONFIG_BUTTON = 0;
	int EXIT_BUTTON = 1;
	int QUICK_LOAD_BUTTON = 2;
	int LOAD_BUTTON = 3;
	int QUICK_SAVE_BUTTON = 4;
	int SAVE_BUTTON = 5;
	int AUTO_BUTTON = 6;
	int SKIP_BUTTON = 7;
};
