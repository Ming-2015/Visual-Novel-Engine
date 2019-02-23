#include "Button.h"

bool Button::onLoad()
{
	if (!texture.loadFromFile(texPath))
	{
		LOGGER->Log("Button", "Error loading button texture");
		return false;
	}
	sprite.setTexture(texture);
	sprite.setPosition(xPos, yPos);
	spriteHover.setTexture(texture);
	spriteHover.setPosition(xPos, yPos);
	texWidth = sprite.getLocalBounds().width;
	texHeight = sprite.getLocalBounds().height;
	hasShader = false;
	pressed = false;
	clicked = false;
	hovered = false;

	if (vertShaderPath != "" && fragShaderPath != "")
	{
		if (!shader.loadFromFile(vertShaderPath, fragShaderPath))
		{
			LOGGER->Log("Button", "Error loading vert/frag shaders");
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
			LOGGER->Log("Button", "Error loading vert shader");
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
			LOGGER->Log("Button", "Error loading frag shader");
		}
		else
		{
			hasShader = true;
		}
	}

	return true;
}

void Button::onUpdate(float delta_t)
{
}

void Button::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (hovered == false)
	{
		if (hasShader)
		{
			states.shader = &shader;
		}
		target.draw(sprite, states);
	}
	if (hovered == true)
	{
		if (hasShader)
		{
			states.shader = &shader;
		}
		target.draw(spriteHover, states);
	}
}

void Button::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
}

sf::Texture& Button::getTexure()
{
	return texture;
}

sf::Sprite& Button::getSprite()
{
	return sprite;
}

bool Button::isPressed(bool reset)
{
	bool temp = pressed;
	if (reset) pressed = false;
	return temp;
}

bool Button::isClicked(bool reset)
{
	bool temp = clicked;
	if (reset) clicked = false;
	return temp;
}
