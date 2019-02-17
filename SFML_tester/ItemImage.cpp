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
	cerr << "ItemImage: " << GLOBAL->ImageRoot;
	return GLOBAL->ImageRoot + name + "/" + expression;
}

ItemImage::ItemImage(string name, string expression, float xPos, float yPos)
	: name(name), expression(expression), xPos(xPos), yPos(yPos)
{
	color = sf::Color(255.f, 255.f, 255.f, alpha);
	setImage();
	sprite.setColor(color);
	this->xPos += sprite.getLocalBounds().width / 2.0f;
	this->yPos += sprite.getLocalBounds().height / 2.0f;
	sprite.setPosition(this->xPos, this->yPos);
	sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	fading = false;
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
	xPos += xOffset;
	yPos += yOffset;
	sprite.setPosition(xPos, yPos);
}

void ItemImage::rotate(bool clockwise, float angle_degree)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	rotationDegree += clockwise ? angle_degree : 360.f - angle_degree;
	sprite.setRotation(rotationDegree);
}

void ItemImage::scale(float xOffset, float yOffset)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height);
	xScale *= xOffset;
	yScale *= yOffset;
	sprite.setScale(xScale, yScale);
}

void ItemImage::setPosition(float xPos, float yPos)
{
	this->xPos = xPos + sprite.getLocalBounds().width /2.0f;
	this->yPos = yPos + sprite.getLocalBounds().height /2.0f;
	sprite.setPosition(xPos, yPos);
}

void ItemImage::setRotation(bool clockwise, float angle_degree)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	rotationDegree = clockwise ? angle_degree : 360.f - angle_degree;
	sprite.setRotation(rotationDegree);
}

void ItemImage::setScale(float xScale, float yScale)
{
	sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	this->xScale = xScale;
	this->yScale = yScale;
	sprite.setScale(xScale, yScale);
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

TextboxImage::TextboxImage()
	: ItemImage("textbox", "textboxw.png", 0, 0)
{
	initText();
}

void TextboxImage::setText(const std::string & s)
{
	dialogue = s;
	dialogueText.setString(dialogue);
}

void TextboxImage::setName(const std::string & s)
{
	name = s;
	nameText.setString(name);
}

void TextboxImage::setDisplay(const std::string & s_name, const std::string & s_dialogue)
{
	setText(s_dialogue);
	setName(s_name);
}

void TextboxImage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	ItemImage::draw(target, states);
	target.draw(dialogueText, states);
	target.draw(nameText, states);
}

void TextboxImage::setTextboxColor(sf::Color color)
{
	sprite.setColor(color);
}

void TextboxImage::initText()
{
	dialogueTextColor = sf::Color(255, 255, 255);
	dialogueFont = sf::Font();
	if (!dialogueFont.loadFromFile(GLOBAL->DisplayTextFont))
	{
		LOGGER->Log("TextboxImage", "Unable to load default font");
	}

	dialogueText.setFont(dialogueFont);
	dialogueText.setStyle(sf::Text::Regular);
	dialogueText.setFillColor(dialogueTextColor);
	dialogueText.setCharacterSize(32);
	dialogueText.setPosition(230.0f, 705.0f);

	nameTextColor = sf::Color(255, 255, 255);
	nameFont = sf::Font();
	if (!nameFont.loadFromFile(GLOBAL->DisplayNameFont))
	{
		LOGGER->Log("TextboxImage", "Unable to load name font");
	}

	nameText.setFont(nameFont);
	nameText.setStyle(sf::Text::Bold);
	nameText.setFillColor(nameTextColor);
	nameText.setCharacterSize(32);
	nameText.setPosition(230.0f, 643.0f);
}

