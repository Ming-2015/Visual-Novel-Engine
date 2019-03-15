#include "ChoiceImage.h"

const float ChoiceImage::TOP_Y = -50;
const float ChoiceImage::BOTTOM_Y = 750;

void ChoiceImage::initText()
{
	choiceTextOutlineColor = sf::Color(0, 0, 0);
	choiceTextColor = sf::Color(255, 255, 255);
	choiceFont = sf::Font();
	if (!choiceFont.loadFromFile(GLOBAL->DisplayTextFont))
	{
		LOGGER->Log("ChoiceImage", "Unable to load default font");
	}

	choiceText.setFont(choiceFont);
	choiceText.setStyle(sf::Text::Regular);
	choiceText.setFillColor(choiceTextColor);
	choiceText.setOutlineColor(choiceTextOutlineColor);
	choiceText.setOutlineThickness(3);
	choiceText.setCharacterSize(32);

	choiceText.setString(choice);

	choiceText.setOrigin(choiceText.getLocalBounds().width / 2.0f, 
		choiceText.getLocalBounds().height / 2.0f);

	choiceText.setPosition(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width / 2.0f,
		sprite.getGlobalBounds().top + sprite.getGlobalBounds().height / 2.0f );

}

ChoiceImage::ChoiceImage(const std::string & str, const std::string & flag, int numChoices, int index)
	: ItemImage("assets", "choicebox_fg.png", 0, 0), 
	choice(str), numChoices(numChoices), index(index)
{
	xPos = CONFIG->getWindowWidth() / 2.0f ;
	yPos = TOP_Y + (BOTTOM_Y - TOP_Y) / float(numChoices + 1) * float(index + 1);

	sprite.setPosition(xPos, yPos);

	if (!choiceboxBgTex.loadFromFile(GLOBAL->AssetRoot + "choicebox_bg.png"))
	{
		LOGGER->Log("ChoiceImage", "Unable to log choice background image");
	}
	choiceboxBgTex.setSmooth(true);
	choiceboxBg.setTexture(choiceboxBgTex);
	choiceboxBg.setPosition(xPos, yPos);
	choiceboxBg.setOrigin(choiceboxBg.getLocalBounds().width / 2.0f, choiceboxBg.getLocalBounds().height / 2.0f);
	
	setChoiceboxColor(sf::Color(255, 0, 180));
	setAlpha(190.f);

	initText();

	flags = UTILITY->split(flag, ',');
	flags = UTILITY->trim(flags);
}

ChoiceImage::ChoiceImage(ifstream & file)
	:ItemImage(file)
{
	initText();

	// note that we can't actually return to the init file position
	int currentPos = file.tellg();

	try
	{
		choice = UTILITY->readFromBinaryFile(file);

		flags = UTILITY->readVectorFromBinaryFile(file);

		file.read(reinterpret_cast<char *>(&numChoices), sizeof(numChoices));
		file.read(reinterpret_cast<char *>(&index), sizeof(index));

		file.read(reinterpret_cast<char *>(&choiceTextColor.r), sizeof(choiceTextColor.r));
		file.read(reinterpret_cast<char *>(&choiceTextColor.g), sizeof(choiceTextColor.g));
		file.read(reinterpret_cast<char *>(&choiceTextColor.b), sizeof(choiceTextColor.b));
		file.read(reinterpret_cast<char *>(&choiceTextColor.a), sizeof(choiceTextColor.a));

		file.read(reinterpret_cast<char *>(&choiceTextOutlineColor.r), sizeof(choiceTextOutlineColor.r));
		file.read(reinterpret_cast<char *>(&choiceTextOutlineColor.g), sizeof(choiceTextOutlineColor.g));
		file.read(reinterpret_cast<char *>(&choiceTextOutlineColor.b), sizeof(choiceTextOutlineColor.b));
		file.read(reinterpret_cast<char *>(&choiceTextOutlineColor.a), sizeof(choiceTextOutlineColor.a));

	}
	catch (exception e)
	{
		LOGGER->Log("ChoiceImage", "Unable to read from save data");
		file.seekg(currentPos);
		loaded = false;
		throw;
	}

	xPos = CONFIG->getWindowWidth() / 2.0f;
	yPos = TOP_Y + (BOTTOM_Y - TOP_Y) / float(numChoices + 1) * float(index + 1);

	sprite.setPosition(xPos, yPos);

	if (!choiceboxBgTex.loadFromFile(GLOBAL->AssetRoot + "choicebox_bg.png"))
	{
		LOGGER->Log("ChoiceImage", "Unable to log choice background image");
	}
	choiceboxBgTex.setSmooth(true);
	choiceboxBg.setTexture(choiceboxBgTex);
	choiceboxBg.setPosition(xPos, yPos);
	choiceboxBg.setOrigin(choiceboxBg.getLocalBounds().width / 2.0f, choiceboxBg.getLocalBounds().height / 2.0f);

	setChoiceboxColor(sf::Color(255, 0, 180));
	setAlpha(190.f);

	initText();
}

void ChoiceImage::serialize(ofstream & savefile)
{
	ItemImage::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, choice);

	UTILITY->writeVectorToBinaryFile(savefile, flags);

	savefile.write(reinterpret_cast<const char *>(&numChoices), sizeof(numChoices));
	savefile.write(reinterpret_cast<const char *>(&index), sizeof(index));

	savefile.write(reinterpret_cast<const char *>(&choiceTextColor.r), sizeof(choiceTextColor.r));
	savefile.write(reinterpret_cast<const char *>(&choiceTextColor.g), sizeof(choiceTextColor.g));
	savefile.write(reinterpret_cast<const char *>(&choiceTextColor.b), sizeof(choiceTextColor.b));
	savefile.write(reinterpret_cast<const char *>(&choiceTextColor.a), sizeof(choiceTextColor.a));

	savefile.write(reinterpret_cast<const char *>(&choiceTextOutlineColor.r), sizeof(choiceTextOutlineColor.r));
	savefile.write(reinterpret_cast<const char *>(&choiceTextOutlineColor.g), sizeof(choiceTextOutlineColor.g));
	savefile.write(reinterpret_cast<const char *>(&choiceTextOutlineColor.b), sizeof(choiceTextOutlineColor.b));
	savefile.write(reinterpret_cast<const char *>(&choiceTextOutlineColor.a), sizeof(choiceTextOutlineColor.a));

}

std::vector<std::string> ChoiceImage::getFlags() const
{
	return flags;
}

std::string ChoiceImage::getText() const
{
	return choice;
}

void ChoiceImage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(choiceboxBg, states);
	ItemImage::draw(target, states);
	target.draw(choiceText, states);
}

void ChoiceImage::setAlpha(float alpha)
{
	ItemImage::setAlpha(alpha);

	choiceboxBg.setColor(color);

	choiceTextColor.a = (sf::Uint8) alpha;
	choiceTextOutlineColor.a = (sf::Uint8) alpha;
	choiceText.setFillColor(choiceTextColor);
	choiceText.setOutlineColor(choiceTextOutlineColor);

}

void ChoiceImage::addAlpha(float alpha)
{
	ItemImage::addAlpha(alpha);

	choiceboxBg.setColor(color);

	choiceTextColor.a += (sf::Uint8) alpha;
	choiceTextOutlineColor.a += (sf::Uint8) alpha;
	choiceText.setFillColor(choiceTextColor);
	choiceText.setOutlineColor(choiceTextOutlineColor);

}

void ChoiceImage::setPosition(float x, float y)
{
	ItemImage::setPosition(x, y);

	choiceboxBg.setPosition(xPos, yPos);
}

void ChoiceImage::move(float x, float y)
{
	ItemImage::move(x, y);

	choiceboxBg.setPosition(xPos, yPos);
}

void ChoiceImage::setChoiceboxColor(sf::Color color)
{
	this->color = color;
	this->color.a = alpha;
	sprite.setColor(this->color);

	choiceboxBg.setColor(this->color);
}

