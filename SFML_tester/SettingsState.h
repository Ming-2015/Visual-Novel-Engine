#pragma once
#include <string>

#include "GameState.h"
#include "Button.h"
#include "Slider.h"
#include "Button.h"
#include "MenuButton.h"
#include "RadioButton.h"
#include "DarkenButton.h"
#include "Checkbox.h"

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

	int currentPage = DG;

	std::vector<Slider *> sliders;
	std::vector<sf::Text> texts;
	std::vector<MenuButton*> zeroButtons;
	std::vector<MenuButton*> hundredButtons;
	std::vector<DarkenButton*> staticDButtons;

	// the various radio buttons
	std::vector<RadioButton *> displayOptionButtons;
	RadioButton* fullScreenButton;
	RadioButton* windowedButton;
	RadioButton* borderlessButton;

	std::vector<CheckBox *> gameFeatures;
	CheckBox* skipUnreadText = nullptr;
	CheckBox* stopSkippingAtChoice = nullptr;
	CheckBox* stopVoiceAtNewLine = nullptr;

	sf::Font settingsFont;
	sf::Texture configTexture;
	sf::Sprite configBackground;
	sf::Texture DGTexture;
	sf::Sprite DGBackground;
	sf::Texture VGTexture;
	sf::Sprite VGBackground;


	const static int SLIDER_MANUAL_SPEED = 0;
	const static int SLIDER_AUTO_SPEED = 1;
	const static int SLIDER_AUTO_WAIT_TIME = 2;
	const static int SLIDER_ALPHA = 3;
	const static int SLIDER_MASTER = 4;
	const static int SLIDER_BGM = 5;
	const static int SLIDER_VOICE = 6;
	const static int SLIDER_SFX = 7;
	
	const static int TEXT_MASTER = 0;
	const static int TEXT_BGM = 1;
	const static int TEXT_VOICE = 2;
	const static int TEXT_SFX = 3;
	const static int TEXT_ALPHA = 4;
	const static int TEXT_SAVE = 5;

	const static int DG = 0;
	const static int VG = 1;
	const static int SAVE = 4;
	const static int RETURN = 3;
	const static int DEFAULT = 2;
};