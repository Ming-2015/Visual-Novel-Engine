#pragma once
#include "Effect.hpp"
#include <string>

class Button : public Effect
{
public:
	// in case there's no frag shader or vert shader, just pass in empty string "" respectively
	Button(std::string texPath, std::string vertShaderPath, 
		std::string fragShaderPath, float xPos, float yPos, 
		float r_width, float r_height) :
		Effect("Button")
	{
		this->texPath = texPath;
		this->xPos = xPos;
		this->yPos = yPos;
		this->buttonWidth = r_width;
		this->buttonHeight = r_height;
		this->vertShaderPath = vertShaderPath;
		this->fragShaderPath = fragShaderPath;
	}

	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event& event, sf::Window& window);
	sf::Texture& getTexure();
	sf::Sprite& getSprite();

	float xPos, yPos;
	float buttonWidth, buttonHeight;
	int texWidth, texHeight;		// actual texture width and height

protected:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Shader shader;

	std::string texPath;
	std::string vertShaderPath;
	std::string fragShaderPath;
	bool hasShader;
};