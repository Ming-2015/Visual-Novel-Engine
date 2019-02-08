#pragma once
#include "Button.h"
#include <string>

class MenuButton : public Button
{
	// in case there's no frag shader or vert shader, just pass in empty string "" respectively
	MenuButton(std::string texPath, std::string vertShaderPath,
		std::string fragShaderPath, float xPos, float yPos,
		float r_width, float r_height) :
		Button(texPath, vertShaderPath, fragShaderPath, xPos, yPos, r_width, r_height)
	{
	}

	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states);
	void handleInput(sf::Event&e, sf::RenderWindow& window);
};