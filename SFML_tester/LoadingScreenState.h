#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "GameState.h"
#include "Logger.h"
#include "ResourceManager.h"

class LoadingScreenState : public GameState
{
public:
	void handleInput(sf::Event& e, sf::RenderWindow& window);				
	void render(sf::RenderWindow& window);		
	void update(float delta_t);
	void init();
	void cleanup();

	LoadingScreenState();

private:

	sf::Text progressText;
	std::string progressStr;

	sf::Sprite background;

	float timer = 0;
};