#include "LogBar.h"

LogBar::LogBar()
	: Effect("LogBar")
{
}

LogBar::~LogBar()
{
}

void LogBar::setName(const std::string & title)
{
	nameText.setString(title);
	nameText.setOrigin(nameText.getLocalBounds().width / 2, nameText.getLocalBounds().height / 2);
}

void LogBar::setLine(const std::string & title)
{
	lineText.setString( UTILITY->addAllNewLines( title, 70) );
}

void LogBar::setPosition(int x, int y)
{
	bgSprite.setPosition(x, y);
	nameText.setPosition(x + nameTextPosX, y + nameTextPosY);
	lineText.setPosition(x + lineTextPosX, y + lineTextPosY);
}

bool LogBar::isClicked(bool reset)
{
	bool temp = clicked;
	if (reset)
	{
		clicked = false;
	}
	return temp;
}

bool LogBar::isPressed() const
{
	return pressed;
}

bool LogBar::isOnHover() const
{
	return hovered;
}

bool LogBar::onLoad()
{
	// initialize the empty text
	if (!font.loadFromFile(GLOBAL->UserInterfaceFont))
	{
		LOGGER->Log("LogBar", "Unable to load font!");
	}

	nameText.setFont(font);
	nameText.setStyle(sf::Text::Bold);
	nameText.setOutlineThickness(3);
	nameText.setFillColor(sf::Color(255, 255, 255, 255));
	nameText.setOutlineColor(sf::Color(0, 0, 0, 255));
	nameText.setCharacterSize(36);

	lineText.setFont(font);
	lineText.setStyle(sf::Text::Bold);
	lineText.setOutlineThickness(2);
	lineText.setFillColor(sf::Color(255, 255, 255, 255));
	lineText.setOutlineColor(sf::Color(0, 0, 0, 255));
	lineText.setCharacterSize(24);

	if (!bgUnselectedTex.loadFromFile(GLOBAL->AssetRoot + "logBar.png"))
	{
		LOGGER->Log("LogBar", "Unable to load images/assets/logBar.png");
	}

	if (!bgSelectedTex.loadFromFile(GLOBAL->AssetRoot + "logBar_selected.png"))
	{
		LOGGER->Log("LogBar", "Unable to load images/assets/logBar_selected.png");
	}

	bgSprite.setTexture(bgUnselectedTex);

	nameText.setPosition(nameTextPosX, nameTextPosY);
	lineText.setPosition(lineTextPosX, lineTextPosY);

	return true;
}

void LogBar::onUpdate(float delta_t)
{

}

void LogBar::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(bgSprite, states);
	target.draw(nameText, states);
	target.draw(lineText, states);
}

void LogBar::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
	switch (e.type)
	{
	case sf::Event::MouseMoved:
	{
		if (bgSprite.getGlobalBounds().contains(mousePosF))
		{
			hovered = true;
			bgSprite.setTexture(bgSelectedTex);
		}
		else
		{
			hovered = false;
			bgSprite.setTexture(bgUnselectedTex);
		}
		break;
	}

	case sf::Event::MouseButtonPressed:
	{
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			if (bgSprite.getGlobalBounds().contains(mousePosF))
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