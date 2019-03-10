#pragma once
#include <string>

#include "GameState.h"
#include "Button.h"
#include "Slider.h"
#include "Button.h"
#include "MenuButton.h"
#include "RadioButton.h"

class SettingsState : public GameState
{
public:
	SettingsState();
	~SettingsState();

	// handle user inputs (keyboard/gamepad/mouse/etc)
	void handleInput(sf::Event& e, sf::RenderWindow& window);

	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void render(sf::RenderWindow& window);

	// all non-rendering game logics/mechanics should be implemented here
	void update(float delta_t);

	// initialize instance
	void init();	
	
	void cleanup();

private:

	std::vector<Slider *> sliders;
	std::vector<sf::Text> texts;
	std::vector<MenuButton*> zeroButtons;
	std::vector<MenuButton*> hundredButtons;

	// the various radio buttons
	std::vector<RadioButton *> enableFullscreenRadioButtons;
	RadioButton* fullScreenButton;
	RadioButton* windowedButton;

	sf::Font settingsFont;
	sf::Texture configTexture;
	sf::Sprite configBackground;

	const static int SLIDER_MASTER = 0;
	const static int SLIDER_BGM = 1;
	const static int SLIDER_VOICE = 2;
	const static int SLIDER_SFX = 3;
	const static int SLIDER_ALPHA = 4;

	const static int TEXT_MASTER = 0;
	const static int TEXT_BGM = 1;
	const static int TEXT_VOICE = 2;
	const static int TEXT_SFX = 3;
	const static int TEXT_ALPHA = 4;
	const static int TEXT_SAVE = 5;
};