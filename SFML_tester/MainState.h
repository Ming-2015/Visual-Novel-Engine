#pragma once
#include "GameState.h"
#include "ScriptLine.h"
#include "ScriptManager.h"
#include "Global.h"

class MainState : public GameState
{
public:

	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);								// all non-rendering game logics/mechanics should be implemented here
	void init();

	const int myState = GameState::STATE_MAIN;
	const ScriptManager* getScriptManager();

	MainState(std::string filename, int lineId);
	~MainState();

private:

	sf::Clock clock;
	unsigned int character;
	ScriptManager* scriptManager;

	sf::Text displayNameStr;
	sf::Font displayNameFont;
	sf::Color displayNameColor;

	sf::Texture background;
	sf::Sprite displayBackground;

	sf::Color displayTextColor;
	sf::Font displayTextFont;
	sf::Text displayTextStr;
};