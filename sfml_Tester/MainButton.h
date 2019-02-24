#pragma once
#include "Button.h"
#include <string>
using namespace std;

class MainButton : public Button
{
public:
	// in case there's no frag shader or vert shader, just pass in empty string "" respectively
	MainButton(std::string texPath, std::string vertShaderPath,
		std::string fragShaderPath, float xPos, float yPos,
		float r_width, float r_height,
		float startX, float startY, float endX, float endY, float startX2, float startY2, float endX2, float endY2) :
		Button(texPath, vertShaderPath, fragShaderPath, xPos, yPos, r_width, r_height),
		startSpriteSheetXPos(startX),
		startSpriteSheetYPos(startY),
		endSpriteSheetXPos(endX),
		endSpriteSheetYPos(endY),
		startSpriteSheetXPos2(startX2),
		startSpriteSheetYPos2(startY2),
		endSpriteSheetXPos2(endX2),
		endSpriteSheetYPos2(endY2)
	{}

	void setAlpha(float alpha);
	void addAlpha(float offset);

protected:
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);
	void setSpriteSheetPos(float startX, float startY, float endX, float endY);
	void setSpriteSheetPos2(float startX, float startY, float endX, float endY);

private:
	float alpha = 255.f;
	float scaleMultiply;
	sf::Clock clock;
	float startSpriteSheetXPos;
	float startSpriteSheetYPos;
	float endSpriteSheetXPos;
	float endSpriteSheetYPos;
	float startSpriteSheetXPos2;
	float startSpriteSheetYPos2;
	float endSpriteSheetXPos2;
	float endSpriteSheetYPos2;
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