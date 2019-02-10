#pragma once
#include "Effect.hpp"
#include <string>

class Slider : public Effect
{
public:
	// in case there's no frag shader or vert shader, just pass in empty string "" respectively
	Slider( std::string sliderTexPath,
		std::string knobTexPath,
		std::string vertShaderPath,
		std::string fragShaderPath,
		float xPos, float yPos,			// position of the slider
		float sliderWidth, float sliderHeight, // width and height of the slider
		float knobWidth, float knobHeight, // width and height of the slider
		float initialValue) :
		// ----- Initialize member variables -----
		Effect("Button"),
		sliderTexPath(sliderTexPath),
		knobTexPath(knobTexPath),
		vertShaderPath(vertShaderPath),
		fragShaderPath(fragShaderPath),
		xPos(xPos),
		yPos(yPos),
		sliderWidth(sliderWidth),
		sliderHeight(sliderHeight),
		knobWidth(knobWidth),
		knobHeight(knobHeight),
		value(initialValue)
	{}

	~Slider();

	static Slider * createSettingsSlider(float xPos, float yPos)
	{
		return new Slider("", "", "assets/rescrollbar.png", "assets/rescrollbar_thumb.png", xPos, yPos, 100.0f, 30.0f, 20.0f, 20.0f, 0.5f);
	}

	float getValue() const; // get the slider position, from 0.0 to 1.0
	float setValue(float value); 

protected:

	// initial values
	std::string vertShaderPath;
	std::string fragShaderPath;
	float xPos, yPos;
	float sliderWidth, sliderHeight;
	float knobWidth, knobHeight;
	float value;
	std::string sliderTexPath;
	std::string knobTexPath;

	// slider texture
	sf::Texture sliderTexture;
	sf::Sprite sliderSprite;

	// knob texture
	sf::Texture knobTexture;
	sf::Sprite knobSprite;

	// actual texture width and height
	int sliderTexWidth, sliderTexHeight;		
	int knobTexWidth, knobTexHeight;

	// prev mouse x position, for offset calculation
	float prevMouseX;
	float knobX, knobY;		// knob position

	sf::Shader shader;
	bool hasShader;

	bool pressed;	// if the slider is currently being pressed
	bool clicked;	// if the slider is being clicked (fully pressed then released)

	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

	float findKnobX(float value);
	float findValue(float knobX);
};