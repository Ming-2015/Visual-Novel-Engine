#pragma once
#include "GameState.h"
#include "ScriptLine.h"
#include "ScriptManager.h"

class MainState : public GameState
{
public:

	void handleInput(sf::Event& e, sf::RenderWindow& window);				// handle user inputs (keyboard/gamepad/mouse/etc)
	void render(sf::RenderWindow& window);		// render the window display: all rendering functions (like drawing images, fonts, etc. should be here)
	void update(float delta_t);								// all non-rendering game logics/mechanics should be implemented here
	void init();

	const int myState = GameState::STATE_MAIN;

	MainState(std::string filename, int lineId);
	~MainState();

private:

	ScriptManager* scriptManager;

	sf::Text displayNameStr;
	sf::Font displayNameFont;
	sf::Color displayNameColor;

	sf::Color displayTextColor;
	sf::Font displayTextFont;
	sf::Text displayTextStr;
};