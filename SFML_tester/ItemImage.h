#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#include "Utility.h"
#include "Global.h"
using namespace std;

class ItemImage : public sf::Drawable{
private:

	// transformation values
	float xPos, yPos;
	float xScale, yScale;
	float rotationDegree;
	bool loaded;

	// for fading
	bool fading;
	float duration;
	string nextExpression;	
	sf::Color nextColor;
	sf::Texture nextTexture;
	sf::Sprite nextSprite;
	
	// name and expression of the character
	string name;
	string expression;

	// texture options
	float alpha = 255.f;
	sf::Color color;
	sf::Texture texture;
	sf::Sprite sprite;

	// helper functions
	void setImage();
	std::string getImagePath(const std::string& name, const std::string& expression) const;

public:
	ItemImage(string name, string expression, float xPos, float yPos);

	void setAlpha(float alpha);
	void addAlpha(float alphaOffset);

	void move(float xOffset, float yOffset);
	void rotate(bool clockwise, float angle_degree);
	void scale(float xOffset, float yOffset);

	void setPosition(float xPos, float yPos);
	void setRotation(bool clockwise, float angle_degree);
	void setScale(float xScale, float yScale);

	void changeExpression(string expression, float time=0);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float delta_t);

	bool isLoaded() const;
	bool isFading() const;

	std::string getName() const;
	std::string getExpression() const;
	float getAlpha() const;
	sf::FloatRect getLocalBoundary() const;
};

class CharacterImage : public ItemImage
{
public:
	CharacterImage(string name, string expression, float xPos, float yPos) :
		ItemImage(name, expression, xPos, yPos) {}
	~CharacterImage() {}
};

class BackgroundImage : public ItemImage
{
public:
	BackgroundImage(string name, string expression, float xPos, float yPos) :
		ItemImage(name, expression, xPos, yPos) {}
	~BackgroundImage() {}
};