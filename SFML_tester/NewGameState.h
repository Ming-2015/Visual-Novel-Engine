#pragma once
#include "GameState.h"
#include <string>

class NewGameState : public GameState
{
public:
	NewGameState();
	~NewGameState();
	
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

	
};

