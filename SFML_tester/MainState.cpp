#include "MainState.h"

void MainState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	switch (e.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			scriptManager->readNextLine();
		}
		break;
	}
}

void MainState::render(sf::RenderWindow & window)
{
	window.draw(displayNameStr);
	window.draw(displayTextStr);
}

void MainState::update(float delta_t)
{
	displayNameStr.setString(scriptManager->getDisplayName());
	displayTextStr.setString(scriptManager->getScriptLine());

	if (scriptManager->eof())
	{
		shouldChangeState = true;
		nextState = GameState::STATE_MENU;
	}
}

void MainState::init()
{
	scriptManager->init();
	if (!displayNameFont.loadFromFile("assets/MATURASC.TTF"))
	{
		LOGGER->Log("MainState", "Cannot find font file 1");
	}
	else
	{
		displayNameColor = sf::Color(255, 255, 255);

		displayNameStr.setFont(displayNameFont);
		displayNameStr.setStyle(sf::Text::Bold | sf::Text::Underlined);
		displayNameStr.setString(scriptManager->getDisplayName());
		displayNameStr.setFillColor(displayNameColor);
		displayNameStr.setCharacterSize(32);
		displayNameStr.setPosition(100.0f, 600.0f);
	}

	if (!displayTextFont.loadFromFile("assets/default.TTF"))
	{
		LOGGER->Log("MainState", "Cannot find font file 2");
	}
	else
	{
		displayTextColor = sf::Color(255, 255, 255);

		displayTextStr.setFont(displayTextFont);
		displayTextStr.setStyle(sf::Text::Regular);
		displayTextStr.setString(scriptManager->getScriptLine());
		displayTextStr.setFillColor(displayTextColor);
		displayTextStr.setCharacterSize(32);
		displayTextStr.setPosition(125.0f, 700.0f);
	}
}

MainState::MainState(std::string filename, int lineId)
{
	scriptManager = new ScriptManager(filename, lineId);
}

MainState::~MainState()
{
	delete scriptManager;
}
