#pragma once
#include <string>

#include "GameState.h"
#include "Button.h"
#include "ScriptManager.h"
#include "SavefileImage.h"

class LoadState : public GameState
{
public:
	LoadState();
	~LoadState();

	// handle user inputs (keyboard/gamepad/mouse/etc)
	void handleInput(sf::Event& e, sf::RenderWindow& window);

	// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void render(sf::RenderWindow& window);

	// all non-rendering game logics/mechanics should be implemented here
	void update(float delta_t);

	// initialize instance
	void init();

	void cleanup();

	// read image, title and scriptmanager
	bool readSave(const std::string& savefile, 
		sf::Image& image, 
		std::string& title, 
		ScriptManager*& scriptManager);

	// read only the image and title
	bool readSave(const std::string& savefile,
		sf::Image& image,
		std::string& title);

	const static int INDEX_SAVE_1 = 0;
	const static int INDEX_SAVE_2 = 1;
	const static int INDEX_SAVE_3 = 2;
	const static int INDEX_SAVE_4 = 3;
	const static int INDEX_SAVE_5 = 4;
	const static int INDEX_SAVE_6 = 5;

private:
	sf::Texture saveTexture;
	sf::Sprite saveBackground;
	sf::Font settingsFont;
	sf::Text startNew;
	sf::Text returnState;

	unsigned int currentPageNumber = 0;
	const unsigned int savePerPage = 6;

	std::vector<SavefileImage *> savefileImages;

	ScriptManager* scriptManager;

	void loadSavesByPage(int pageNumber);
};