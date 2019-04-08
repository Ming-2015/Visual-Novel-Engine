#include "Slider.h"

Slider::~Slider()
{
}

float Slider::getValue() const
{
	return value;
}

float Slider::setValue(float p_value)
{
	value = p_value;
	if (value > 1.0f) value = 1.0f;
	else if (value < 0.0f) value = 0.0f;

	knobX = findKnobX(value);
	knobSprite.setPosition(knobX, knobY);

	return 0.0f;
}

bool Slider::onLoad()
{
	if (!sliderTexture.loadFromFile(sliderTexPath))
	{
		LOGGER->Log("Slider", "Error loading slider texture");
		return false;
	}
	sliderTexture.setSmooth(true);
	sliderSprite.setTexture(sliderTexture);
	sliderTexWidth = sliderSprite.getLocalBounds().width;
	sliderTexHeight = sliderSprite.getLocalBounds().height;

	float scaleX = sliderWidth / (float)sliderTexWidth ;
	float scaleY = sliderHeight / (float)sliderTexHeight ;
	knobOffset = 2.0;

	//sliderSprite.moves(-sliderTexWidth / 2.0, -sliderTexHeight / 2.0);
	sliderSprite.setScale(scaleX, scaleY);
	sliderSprite.setPosition(xPos, yPos + knobOffset);

	if (!knobTexture.loadFromFile(knobTexPath))
	{
		LOGGER->Log("Slider", "Error loading knob texture");
		return false;
	}
	knobTexture.setSmooth(true);
	knobSprite.setTexture(knobTexture);
	knobTexWidth = knobSprite.getLocalBounds().width;
	knobTexHeight = knobSprite.getLocalBounds().height;

	knobMinX = xPos + knobWidth / 2.0f;
	slidingWidth = sliderWidth - knobWidth;

	scaleX = knobWidth / (float)knobTexWidth;
	scaleY = knobHeight / (float)knobTexHeight;
	knobX = findKnobX(value);
	knobY = yPos + sliderHeight / 2.0f - (knobHeight / 2.0f);

	knobSprite.setScale(scaleX, scaleY);
	knobSprite.setPosition(knobX, knobY);

	hasShader = false;
	pressed = false;
	clicked = false;

	if (vertShaderPath != "" && fragShaderPath != "")
	{
		if (!shader.loadFromFile(vertShaderPath, fragShaderPath))
		{
			LOGGER->Log("Slider", "Error loading vert/frag shaders");
		}
		else
		{
			hasShader = true;
		}
	}
	else if (vertShaderPath != "")
	{
		if (!shader.loadFromFile(vertShaderPath, sf::Shader::Vertex))
		{
			LOGGER->Log("Slider", "Error loading vert shader");
		}
		else
		{
			hasShader = true;
		}
	}
	else if (fragShaderPath != "")
	{
		if (!shader.loadFromFile(fragShaderPath, sf::Shader::Fragment))
		{
			LOGGER->Log("Slider", "Error loading frag shader");
		}
		else
		{
			hasShader = true;
		}
	}

	return true;
}

void Slider::onUpdate(float delta_t)
{
}

void Slider::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (hasShader)
	{
		states.shader = &shader;
	}
	target.draw(sliderSprite, states);
	target.draw(knobSprite, states);
}

void Slider::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);

	switch (e.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			if (knobSprite.getGlobalBounds().contains(mousePosF))
			{
				pressed = true;
			}
			else if (sliderSprite.getGlobalBounds().contains(mousePosF))
			{
				pressed = true;
				knobX = mousePosF.x - knobWidth / 2.0f;

				if (knobX < findKnobX(0)) knobX = findKnobX(0);
				else if (knobX > findKnobX(1.0f)) knobX = findKnobX(1.0f);

				knobSprite.setPosition(knobX, knobY);
				value = findValue(knobX);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			if (pressed)
			{
				// find the x offset
				knobX = mousePosF.x - knobWidth / 2.0f;

				// fix the knob on the slider
				if (knobX < findKnobX(0))
				{
					knobX = findKnobX(0);
				}
				else if (knobX > findKnobX(1.0f))
				{
					knobX = findKnobX(1.0f);
				}

				// reset the knob position and set the value
				knobSprite.setPosition(knobX, knobY);
				value = findValue(knobX);

				//string msg = "KnobX = " + to_string(knobX) + "; value = " + to_string(value);
				//LOGGER->Log("Slider", msg);
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			if (pressed)
			{
				clicked = true;
				pressed = false;
			}
			break;
		}
	}
}

float Slider::findKnobX(float value)
{
	return knobMinX - (knobWidth / 2.0f) + (slidingWidth * value);
}

float Slider::findValue(float knobX)
{
	return (knobX - knobMinX + knobWidth / 2.0f)/ slidingWidth;
}

void Slider::move(float x, float y)
{
	this->xPos += x;
	this->yPos += y;
	this->knobX += x;
	this->knobY += y;
	sliderSprite.setPosition(xPos, yPos + knobOffset);
	knobSprite.setPosition(knobX, knobY);
}

