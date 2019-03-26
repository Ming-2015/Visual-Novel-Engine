#include "SavefileImage.h"

SavefileImage::SavefileImage(int y)
	: Effect("SavefileImage")
{
	x = y;
}

SavefileImage::~SavefileImage()
{
}

void SavefileImage::setImage(const sf::Image & image)
{
	saveTex.create(image.getSize().x, image.getSize().y);
	saveTex.update(image);

	saveSprite.setTexture(saveTex);
	setToScale();
}

void SavefileImage::setTitle(const std::string & title)
{
	saveTitle = title;
	if (saveTitle.length() >= stringLength)
	{
		saveTitle = saveTitle.substr(0, stringLength-3);
		saveTitle += "...";
	}
	saveTitleText.setString(saveTitle);
}

void SavefileImage::setDate(const std::string & date)
{
	saveDate = date;
	if (saveDate.length() >= stringLength)
	{
		saveDate = saveDate.substr(0, stringLength - 3);
		saveDate += "...";
	}
	saveDateText.setString(saveDate);
}

void SavefileImage::useDefaultSprite()
{
	if (!saveTex.loadFromFile("images/assets/nodata.png"))
	{
		LOGGER->Log("SavefileImage", "Unable to load images/assets/nodata.png");
	}
	saveTex.setSmooth(true);
	if (!noDataTex.loadFromFile("images/assets/noDataText.png"))
	{
		LOGGER->Log("SavefileImage", "Unable to load images/assets/noDataText.png");
	}
	noDataTex.setSmooth(true);

	if (x == 1 || x == 5)
	{
		if (!bgBoxUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "noDataBox1.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/noDataBox1.png");
		}
		bgBoxUnselectedTex.setSmooth(true);
	}
	else if (x == 2 || x == 6)
	{
		if (!bgBoxUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "noDataBox2.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/noDataBox1.png");
		}
		bgBoxUnselectedTex.setSmooth(true);
	}
	else if (x == 3 || x == 7)
	{
		if (!bgBoxUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "noDataBox3.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/noDataBox3.png");
		}
		bgBoxUnselectedTex.setSmooth(true);
	}
	else if (x == 4 || x == 8)
	{
		if (!bgBoxUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "noDataBox4.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/noDataBox4.png");
		}
		bgBoxUnselectedTex.setSmooth(true);
	}


	noDataSprite.setTexture(noDataTex);
	noDataSpriteBox.setTexture(saveTex);
	setToScale();
}

void SavefileImage::setPosition(int x, int y)
{
	bgSprite.setPosition(x - 5, y - 14);
	bgBoxSprite.setPosition(x - 15, y - 13);
	saveSprite.setPosition(x + borderSize - 5, y + borderSize - 14);
	saveTitleText.setPosition(x + 300, y + 11);
	saveDateText.setPosition(x + 300, y + 102);
	noDataSprite.setPosition(x + 352, y + 8);
	noDataSpriteBox.setPosition(x + borderSize - 5, y + borderSize - 14);
}

bool SavefileImage::isClicked(bool reset)
{
	bool temp = clicked;
	if (reset)
	{
		clicked = false;
	}
	return temp;
}

bool SavefileImage::isPressed() const
{
	return pressed;
}

bool SavefileImage::isOnHover() const
{
	return hovered;
}

bool SavefileImage::onLoad()
{
	// initialize the empty text
	if (!font.loadFromFile(GLOBAL->UserInterfaceFont))
	{
		LOGGER->Log("SavefileImage", "Unable to load font!");
	}

	saveTitleText.setFont(font);
	saveTitleText.setStyle(sf::Text::Regular);
	saveTitleText.setFillColor(sf::Color(255, 255, 255, 255));
	saveTitleText.setCharacterSize(22);
	saveTitleText.setOutlineColor(sf::Color(0, 0, 0, 100));
	saveTitleText.setOutlineThickness(1);

	saveDateText.setFont(font);
	saveDateText.setStyle(sf::Text::Regular);
	saveDateText.setFillColor(sf::Color(255, 255, 255, 255));
	saveDateText.setCharacterSize(22);
	saveDateText.setOutlineColor(sf::Color(0, 0, 0, 100));
	saveDateText.setOutlineThickness(1);

	if (!bgUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "savebox.png"))
	{
		LOGGER->Log("SavefileImage", "Unable to load images/assets/savebox.png");
	}
	bgUnselectedTex.setSmooth(true);
	if (x == 1 || x == 5)
	{
		if (!bgBoxUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "SaveBoxBack1.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/SaveBoxBack1.png");
		}
		bgBoxUnselectedTex.setSmooth(true);
	}
	else if (x == 2 || x == 6)
	{
		if (!bgBoxUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "SaveBoxBack2.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/SaveBoxBack1.png");
		}
		bgBoxUnselectedTex.setSmooth(true);
	}
	else if (x == 3 || x == 7)
	{
		if (!bgBoxUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "SaveBoxBack3.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/SaveBoxBack1.png");
		}
		bgBoxUnselectedTex.setSmooth(true);
	}
	else if (x == 4 || x == 8)
	{
		if (!bgBoxUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "SaveBoxBack4.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/SaveBoxBack1.png");
		}
		bgBoxUnselectedTex.setSmooth(true);
	}
	/*if (!bgBoxSelectedTex.loadFromFile(GLOBAL->AssetRoot + "savebox_selected.png"))
	{
		LOGGER->Log("SavefileImage", "Unable to load images/assets/savebox_selected.png");
	}
	bgBoxSelectedTex.setSmooth(true);*/
	if (x == 1 || x == 5)
	{
		if (!bgBoxSelectedTex.loadFromFile(GLOBAL->AssetRoot + "SaveBoxBack1.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/SaveBoxBack1.png");
		}
		bgBoxSelectedTex.setSmooth(true);
	}
	else if (x == 2 || x == 6)
	{
		if (!bgBoxSelectedTex.loadFromFile(GLOBAL->AssetRoot + "SaveBoxBack2.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/SaveBoxBack1.png");
		}
		bgBoxSelectedTex.setSmooth(true);
	}
	else if (x == 3 || x == 7)
	{
		if (!bgBoxSelectedTex.loadFromFile(GLOBAL->AssetRoot + "SaveBoxBack3.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/SaveBoxBack1.png");
		}
		bgBoxSelectedTex.setSmooth(true);
	}
	else if (x == 4 || x == 8)
	{
		if (!bgBoxSelectedTex.loadFromFile(GLOBAL->AssetRoot + "SaveBoxBack4.png"))
		{
			LOGGER->Log("SavefileImage", "Unable to load images/assets/SaveBoxBack1.png");
		}
		bgBoxSelectedTex.setSmooth(true);
	}

	bgSprite.setTexture(bgUnselectedTex);
	bgBoxSprite.setTexture(bgBoxUnselectedTex);
	setToScale();
	return true;
}

void SavefileImage::onUpdate(float delta_t)
{

}

void SavefileImage::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(bgBoxSprite, states);
	target.draw(bgSprite, states);
	target.draw(saveSprite, states);
	target.draw(saveTitleText, states);
	target.draw(saveDateText, states);
	target.draw(noDataSprite, states);
	target.draw(noDataSpriteBox, states);
;}

void SavefileImage::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{

	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
	switch (e.type)
	{
		case sf::Event::MouseMoved:
		{
			if (bgSprite.getGlobalBounds().contains(mousePosF) || bgBoxSprite.getGlobalBounds().contains(mousePosF))
			{
				hovered = true;
				//bgBoxSprite.setTexture(bgBoxSelectedTex);
				bgBoxSprite.setColor(sf::Color(150, 150, 150, 255));
				saveTitleText.setFillColor(sf::Color(220, 220, 220, 255));
				saveDateText.setFillColor(sf::Color(220, 220, 220, 255));
				noDataSprite.setColor(sf::Color(200, 200, 200, 255));
				noDataSpriteBox.setColor(sf::Color(200, 200, 200, 255));
			}
			else
			{
				hovered = false;
				bgBoxSprite.setTexture(bgBoxUnselectedTex);
				bgBoxSprite.setColor(sf::Color(255, 255, 255, 255));
				saveTitleText.setFillColor(sf::Color(255, 255, 255, 255));
				saveDateText.setFillColor(sf::Color(255, 255, 255, 255));
				noDataSprite.setColor(sf::Color(255, 255, 255, 255));
				noDataSpriteBox.setColor(sf::Color(255, 255, 255, 255));
			}
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				if (bgSprite.getGlobalBounds().contains(mousePosF) || bgBoxSprite.getGlobalBounds().contains(mousePosF))
				{
					pressed = true;
				}
			}
			break;
		}

		case sf::Event::MouseButtonReleased:
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				if (pressed)
				{
					pressed = false;
					clicked = true;
				}
			}
			break;
		}
	}
}

void SavefileImage::setToScale()
{
	float xScale = float(bgSprite.getLocalBounds().width - borderSize * 2) / float(saveSprite.getLocalBounds().width);
	float yScale = float(bgSprite.getLocalBounds().height - borderSize * 2) / float(saveSprite.getLocalBounds().height);

	saveSprite.setScale(xScale, yScale);
}

void SavefileImage::resetDefault()
{
	if (!noDataTex.loadFromFile("images/assets/empty.png"))
	{
		LOGGER->Log("SavefileImage", "Unable to load images/assets/empty.png");
	}
	//noDataSprite.setTexture(noDataTex);
	setToScale();
}