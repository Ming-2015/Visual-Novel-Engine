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

	setPosition( xPos, yPos);
	sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	fading = false;

	rotationDegree = 0;
	xScale = 1.0f;
	yScale = 1.0f;
}

ItemImage::ItemImage(ifstream & file)
{
	int startPos = file.tellg();

	try {
		// write stuffs!
		file.read(reinterpret_cast<char *>(&xPos), sizeof(xPos));
		file.read(reinterpret_cast<char *>(&yPos), sizeof(yPos));
		file.read(reinterpret_cast<char *>(&xScale), sizeof(xScale));
		file.read(reinterpret_cast<char *>(&yScale), sizeof(yScale));
		file.read(reinterpret_cast<char *>(&rotationDegree), sizeof(rotationDegree));
		file.read(reinterpret_cast<char *>(&loaded), sizeof(loaded));

		file.read(reinterpret_cast<char *>(&fading), sizeof(fading));
		file.read(reinterpret_cast<char *>(&duration), sizeof(duration));
		file.read(reinterpret_cast<char *>(&finalAlpha), sizeof(finalAlpha));
		nextExpression = UTILITY->readFromBinaryFile(file);
		file.read(reinterpret_cast<char *>(&nextColor.r), sizeof(nextColor.r));
		file.read(reinterpret_cast<char *>(&nextColor.g), sizeof(nextColor.g));
		file.read(reinterpret_cast<char *>(&nextColor.b), sizeof(nextColor.b));
		file.read(reinterpret_cast<char *>(&nextColor.a), sizeof(nextColor.a));
		file.read(reinterpret_cast<char *>(&nextXPos), sizeof(nextXPos));
		file.read(reinterpret_cast<char *>(&nextYPos), sizeof(nextYPos));

		name = UTILITY->readFromBinaryFile(file);
		expression = UTILITY->readFromBinaryFile(file);
		file.read(reinterpret_cast<char *>(&alpha), sizeof(alpha));
		file.read(reinterpret_cast<char *>(&color.r), sizeof(color.r));
		file.read(reinterpret_cast<char *>(&color.g), sizeof(color.g));
		file.read(reinterpret_cast<char *>(&color.b), sizeof(color.b));
		file.read(reinterpret_cast<char *>(&color.a), sizeof(color.a));

		int uniformParamsSize;
		file.read(reinterpret_cast<char *>(&uniformParamsSize), sizeof(uniformParamsSize));
		for (int i = 0; i < uniformParamsSize; i++)
		{
			std::string paramName = UTILITY->readFromBinaryFile(file);
			float paramValue;
			file.read(reinterpret_cast<char *>(&paramValue), sizeof(paramValue));
			uniformParams.insert(std::pair<std::string, float>(paramName, paramValue));
		}

		file.read(reinterpret_cast<char *>(&hasShader), sizeof(hasShader));
		fragShaderPath = UTILITY->readFromBinaryFile(file);
		vertShaderPath = UTILITY->readFromBinaryFile(file);
	}
	catch (exception e)
	{
		file.seekg(startPos);
		LOGGER->Log("ItemImage", "Failed to read file!");
		loaded = false;
		throw;
	}

	// initialize the sprite
	setImage();
	color.a = alpha;
	sprite.setColor(color);
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
	sprite.setPosition(xPos, yPos);
	sprite.setRotation(rotationDegree);
	sprite.setScale(xScale, yScale);

	if (fading)
	{
		std::string filename = getImagePath(name, nextExpression);
		if (!nextTexture.loadFromFile(filename))
		{
			LOGGER->Log("ItemImage", "Unable to load next expression");
		}
		nextSprite.setTexture(nextTexture);
		nextSprite.setOrigin(nextSprite.getLocalBounds().width / 2.f, nextSprite.getLocalBounds().height / 2.f);
		nextSprite.setPosition(nextXPos, nextYPos);
		nextSprite.setColor(nextColor);
		nextSprite.setRotation(rotationDegree);
		nextSprite.setScale(xScale, yScale);
	}
	
	if (fragShaderPath != "")
	{
		if (!shader.loadFromFile(fragShaderPath, sf::Shader::Fragment))
		{
			LOGGER->Log("ItemImage", "Unable to load fragment shader");
		}
	}
	if (vertShaderPath != "")
	{
		if (!shader.loadFromFile(vertShaderPath, sf::Shader::Vertex))
		{
			LOGGER->Log("ItemImage", "Unable to load vertex shader");
		}
	}

	for (auto it = uniformParams.begin(); it != uniformParams.end(); it++)
	{
		setShaderParam(it->first, it->second);
	}
}

void ItemImage::setAlpha(float alpha)
{
	float a = alpha;
	if (a < 0) a = 0;
	if (a > 255.f) a = 255.f;
	color.a = a;
	this->alpha = a;
	sprite.setColor(color);

	if (fading) {
		finalAlpha = a;
	}
}

void ItemImage::addAlpha(float alphaOffset)
{
	float a = alpha + alphaOffset;
	if (a < 0) a = 0;
	if (a > 255.f) a = 255.f;
	color.a = a;
	this->alpha = a;
	sprite.setColor(color);

	if (fading)
	{
		finalAlpha = a;
	}
}

void ItemImage::move(float xOffset, float yOffset)
{
	xPos += xOffset;
	yPos += yOffset;
	sprite.setPosition(xPos, yPos);

	if (fading)
	{
		nextXPos += xOffset;
		nextYPos += yOffset;
		nextSprite.setPosition(nextXPos, nextYPos);
	}
}

void ItemImage::rotate(bool clockwise, float angle_degree)
{
	rotationDegree += clockwise ? angle_degree : -angle_degree;
	sprite.setRotation(rotationDegree);

	if (fading)
	{
		nextSprite.setRotation(rotationDegree);
	}
}

void ItemImage::scale(float xOffset, float yOffset, float xPos, float yPos)
{
	move(xPos * (xOffset - 1.f), yPos * (yOffset - 1.f));

	xScale *= xOffset;
	yScale *= yOffset;
	sprite.setScale(xScale, yScale);

	if (fading)
	{
		nextSprite.setScale(xScale, yScale);
	}
}

void ItemImage::setPosition(float xPos, float yPos)
{
	this->xPos = xPos + sprite.getLocalBounds().width /2.0f;
	this->yPos = yPos + sprite.getLocalBounds().height /2.0f;
	sprite.setPosition(this->xPos, this->yPos);

	if (fading)
	{
		nextXPos = xPos + nextSprite.getLocalBounds().width / 2.0f;
		nextYPos = yPos + nextSprite.getLocalBounds().width / 2.0f;
		nextSprite.setPosition(nextXPos, nextYPos);
	}
}

void ItemImage::setRotation(bool clockwise, float angle_degree)
{
	rotationDegree = clockwise ? angle_degree : 360.f - angle_degree;
	sprite.setRotation(rotationDegree);

	if (fading)
	{
		nextSprite.setRotation(rotationDegree);
	}
}

void ItemImage::setScale(float xScale, float yScale, float xPos, float yPos)
{
	move(xPos * (this->xScale - xScale), yPos * (this->yScale - yScale));

	this->xScale = xScale;
	this->yScale = yScale;
	sprite.setScale(xScale, yScale);

	if (fading)
	{
		nextSprite.setScale(xScale, yScale);
	}
}

sf::Vector2f ItemImage::getScale() const
{
	return sprite.getScale();
}

float ItemImage::getShaderParam(std::string uniform) const
{
	auto it = uniformParams.find(uniform);
	if (it != uniformParams.end())
	{
		return (it)->second;
	}

	LOGGER->Log("Item", "Trying to get invalid uniformParams!");
	return 0;
}

void ItemImage::serialize(ofstream & savefile) const
{
	// write stuffs!
	savefile.write(reinterpret_cast<const char *>(&xPos), sizeof(xPos));
	savefile.write(reinterpret_cast<const char *>(&yPos), sizeof(yPos));
	savefile.write(reinterpret_cast<const char *>(&xScale), sizeof(xScale));
	savefile.write(reinterpret_cast<const char *>(&yScale), sizeof(yScale));
	savefile.write(reinterpret_cast<const char *>(&rotationDegree), sizeof(rotationDegree));
	savefile.write(reinterpret_cast<const char *>(&loaded), sizeof(loaded));

	savefile.write(reinterpret_cast<const char *>(&fading), sizeof(fading));
	savefile.write(reinterpret_cast<const char *>(&duration), sizeof(duration));
	savefile.write(reinterpret_cast<const char *>(&finalAlpha), sizeof(finalAlpha));
	UTILITY->writeToBinaryFile(savefile, nextExpression);
	savefile.write(reinterpret_cast<const char *>(&nextColor.r), sizeof(nextColor.r));
	savefile.write(reinterpret_cast<const char *>(&nextColor.g), sizeof(nextColor.g));
	savefile.write(reinterpret_cast<const char *>(&nextColor.b), sizeof(nextColor.b));
	savefile.write(reinterpret_cast<const char *>(&nextColor.a), sizeof(nextColor.a));
	savefile.write(reinterpret_cast<const char *>(&nextXPos), sizeof(nextXPos));
	savefile.write(reinterpret_cast<const char *>(&nextYPos), sizeof(nextYPos));

	UTILITY->writeToBinaryFile(savefile, name);
	UTILITY->writeToBinaryFile(savefile, expression);
	savefile.write(reinterpret_cast<const char *>(&alpha), sizeof(alpha));
	savefile.write(reinterpret_cast<const char *>(&color.r), sizeof(color.r));
	savefile.write(reinterpret_cast<const char *>(&color.g), sizeof(color.g));
	savefile.write(reinterpret_cast<const char *>(&color.b), sizeof(color.b));
	savefile.write(reinterpret_cast<const char *>(&color.a), sizeof(color.a));

	int uniformParamsSize = uniformParams.size();
	savefile.write(reinterpret_cast<const char *>(&uniformParamsSize), sizeof(uniformParamsSize));
	for (auto it = uniformParams.begin(); it != uniformParams.end(); it++)
	{
		UTILITY->writeToBinaryFile(savefile, it->first);
		savefile.write(reinterpret_cast<const char *>(&(it->second)), sizeof(it->second));
	}

	savefile.write(reinterpret_cast<const char *>(&hasShader), sizeof(hasShader));
	UTILITY->writeToBinaryFile(savefile, fragShaderPath);
	UTILITY->writeToBinaryFile(savefile, vertShaderPath);
}

void ItemImage::changeExpression(string expression, float time)
{
	if (nextExpression == expression)
	{
		return;
	}

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
		nextExpression = expression;
		
		fading = true;
		duration = time;
		nextColor = color;
		nextColor.a = 0;
		finalAlpha = color.a;

		std::string filename = getImagePath(name, nextExpression);
		if (!nextTexture.loadFromFile(filename))
		{
			std::string err = "Unable to open character image: " + filename;
			LOGGER->Log("ItemImage", err);
			return;
		}
		nextSprite.setTexture(nextTexture);
		
		nextSprite.setOrigin(nextSprite.getLocalBounds().width / 2.0f, nextSprite.getLocalBounds().height / 2.0f);
		nextXPos = xPos - sprite.getLocalBounds().width / 2.0f + nextSprite.getLocalBounds().width / 2.0f;
		nextYPos = yPos - sprite.getLocalBounds().height / 2.0f + nextSprite.getLocalBounds().height / 2.0f;
		nextSprite.setPosition(nextXPos, nextYPos);
		
		nextSprite.setRotation(rotationDegree);
		nextSprite.setScale(xScale, yScale);
		nextSprite.setColor(nextColor);
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
		if (nextColor.a >= finalAlpha)
		{
			this->expression = nextExpression;
			sprite = nextSprite;
			texture = nextTexture;
			color = nextColor;
			xPos = nextXPos;
			yPos = nextYPos;
			fading = false;
		}
		else
		{
			float alphaOffset = delta_t / duration * finalAlpha;
			if (finalAlpha - nextColor.a <= alphaOffset)
			{
				nextColor.a = finalAlpha;
			}
			else
			{
				nextColor.a += alphaOffset;
			}
			nextSprite.setColor(nextColor);

			//if (color.a >= alphaOffset)
			//{
			//	color.a -= alphaOffset;
			//}
			//else
			//{
			//	color.a = 0;
			//}
			//sprite.setColor(color);
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

sf::FloatRect ItemImage::getGlobalBoundary() const
{
	return sprite.getGlobalBounds();
}

void ItemImage::setFragShader(string src)
{
	if (!shader.loadFromFile(src, sf::Shader::Type::Fragment))
	{
		LOGGER->Log("ItemImage:", "Failed to load fragment shader");
		return;
	}

	fragShaderPath = src;
}

void ItemImage::setVertShader(string src)
{
	if (!shader.loadFromFile(src, sf::Shader::Type::Vertex))
	{
		LOGGER->Log("ItemImage:", "Failed to load vertex shader");
		return;
	}

	vertShaderPath = src;
}

void ItemImage::tickShader(bool isTrue)
{
	hasShader = isTrue;
}

void ItemImage::setShaderParam(std::string uniform, float shaderParam)
{
	uniformParams[uniform] = shaderParam;
	shader.setUniform(uniform, shaderParam);
}
