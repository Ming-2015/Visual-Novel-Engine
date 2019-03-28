#pragma once
#include "Button.h"
#include "Config.h"
#include <string>

class DarkenButton : public Button
{
public:
	// in case there's no frag shader or vert shader, just pass in empty string "" respectively
	DarkenButton(std::string texPath, std::string vertShaderPath,
		std::string fragShaderPath, float xPos, float yPos,
		float r_width, float r_height, float startX, float startY, float endX, float endY) :
		Button(texPath, vertShaderPath, fragShaderPath, xPos, yPos),
		startSpriteSheetXPos(startX),
		startSpriteSheetYPos(startY),
		endSpriteSheetXPos(endX),
		endSpriteSheetYPos(endY)
	{}

	bool highlight;
	bool hoverlight;

protected:
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);
	void setSpriteSheetPos(float startX, float startY, float endX, float endY);

private:
	float scaleMultiply;
	sf::Clock clock;
	float startSpriteSheetXPos;
	float startSpriteSheetYPos;
	float endSpriteSheetXPos;
	float endSpriteSheetYPos;
	
	// Here's all the variables in the button class, for reference
	// sf::Texture texture;
	// sf::Sprite sprite;
	// sf::Shader shader;

	// std::string texPath;
	// std::string vertShaderPath;
	// std::string fragShaderPath;
	// float xPos, yPos;
	// float buttonWidth, buttonHeight;
	// int texWidth, texHeight;		// actual texture width and height
	// bool hasShader;
};