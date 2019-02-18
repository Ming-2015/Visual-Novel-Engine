#include "ItemImage.h"

void ItemImage::setImage()
{
	std::string filename = getImagePath(name, expression);
	if (!texture.loadFromFile(filename))
	{
		std::string err = "Unable to open character image: " + filename;
		LOGGER->Log("ItemImage", err);
		loaded = false;
		return;
	}

	sprite.setTexture(texture);
	loaded = true;
}

std::string ItemImage::getImagePath(const std::string& name, const std::string& expression) const
{
	return GLOBAL->ImageRoot + name + "/" + expression;
}

ItemImage::ItemImage(string name, string expression, float xPos, float yPos)
	: name(name), expression(expression), xPos(xPos), yPos(yPos)
{
	color = sf::Color(255.f, 255.f, 255.f, alpha);
	setImage();
	sprite.setColor(color);

	setPosition( sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	fading = false;

	rotationDegree = 0;
	xScale = 1.0f;
	yScale = 1.0f;
}

void ItemImage::setAlpha(float alpha)
{
	float a = alpha;
	if (a < 0) a = 0;
	if (a > 255.f) a = 255.f;
	color.a = a;
	this->alpha = a;
	sprite.setColor(color);
}

void ItemImage::addAlpha(float alphaOffset)
{
	float a = alpha + alphaOffset;
	if (a < 0) a = 0;
	if (a > 255.f) a = 255.f;
	color.a = a;
	this->alpha = a;
	sprite.setColor(color);
}

void ItemImage::move(float xOffset, float yOffset)
{
	//xPos += xOffset;
	//yPos += yOffset;
	sprite.move(xOffset, yOffset);
}

void ItemImage::rotate(bool clockwise, float angle_degree)
{
	rotationDegree += clockwise ? angle_degree : -angle_degree;
	sprite.setRotation(rotationDegree);
}

void ItemImage::scale(float xOffset, float yOffset, float xPos, float yPos)
{
	move(xPos * (xOffset - 1.f), yPos * (yOffset - 1.f));

	xScale *= xOffset;
	yScale *= yOffset;
	sprite.setScale(xScale, yScale);
}

void ItemImage::scaleRel(float xOffset, float yOffset)
{
	xScale *= xOffset;
	yScale *= yOffset;
	sprite.scale(xScale, yScale);
}

void ItemImage::setPosition(float xPos, float yPos)
{
	this->xPos = xPos + sprite.getLocalBounds().width /2.0f;
	this->yPos = yPos + sprite.getLocalBounds().height /2.0f;
	sprite.setPosition(xPos, yPos);
}

void ItemImage::setRotation(bool clockwise, float angle_degree)
{
	rotationDegree = clockwise ? angle_degree : 360.f - angle_degree;
	sprite.setRotation(rotationDegree);
}

void ItemImage::setScale(float xScale, float yScale, float xPos, float yPos)
{
	move(xPos * (this->xScale - xScale), yPos * (this->yScale - yScale));

	this->xScale = xScale;
	this->yScale = yScale;
	sprite.setScale(xScale, yScale);
}

sf::Vector2f ItemImage::getScale() const
{
	return sprite.getScale();
}

void ItemImage::changeExpression(string expression, float time)
{
	if (fading)
	{
		this->expression = nextExpression;
		setImage();
	}

	if (this->expression == expression)
	{
		return;
	}

	if (time <= 0)
	{
		this->expression = expression;
		setImage();
	}
	else
	{
		std::string filename = getImagePath(name, nextExpression);
		if (!nextTexture.loadFromFile(filename))
		{
			std::string err = "Unable to open character image: " + filename;
			LOGGER->Log("ItemImage", err);
			return;
		}
		nextSprite.setTexture(nextTexture);

		fading = true;
		duration = time;
		nextExpression = expression;
		nextColor = sf::Color(255.f, 255.f, 255.f, alpha);

		nextSprite.setPosition(xPos, yPos);
		
		nextSprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
		nextSprite.setRotation(rotationDegree);

		nextSprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height);
		nextSprite.setScale(xScale, yScale);
	}
}

void ItemImage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (loaded)
	{
		target.draw(sprite, states);
	}
	if (fading)
	{
		target.draw(nextSprite, states);
	}
}

void ItemImage::update(float delta_t)
{
	if (fading)
	{
		if (nextColor.a <= 0)
		{
			this->expression = nextExpression;
			sprite.setTexture(nextTexture);
			texture = nextTexture;
			fading = false;
		}
		else
		{
			float alphaOffset = delta_t / duration * alpha;
			nextColor.a -= alphaOffset;
			if (nextColor.a < 0) nextColor.a = 0;
			else if (nextColor.a > 255.f) nextColor.a = 255.f;
			nextSprite.setColor(nextColor);
		}
	}
}

bool ItemImage::isLoaded() const
{
	return loaded;
}

bool ItemImage::isFading() const
{
	return fading;
}

std::string ItemImage::getName() const
{
	return name;
}

std::string ItemImage::getExpression() const
{
	return expression;
}

float ItemImage::getAlpha() const
{
	return alpha;
}

sf::FloatRect ItemImage::getLocalBoundary() const
{
	return sprite.getLocalBounds();
}
