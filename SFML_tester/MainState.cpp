#include "MainState.h"

void MainState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	switch (e.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			scriptManager->readNextLine();
			character = 0;
			if (!scriptManager->getBackgroundChange()) {
				if (!background.loadFromFile(scriptManager->getBackgroundFileName()))
					LOGGER->Log("MenuState", "Unable to get Background Image");
				displayBackground.setTexture(background);
			}}
		break;
	}
}

void MainState::render(sf::RenderWindow & window)
{
	if (clock.getElapsedTime().asMilliseconds() > 15 && character < scriptManager->getScriptLine().length())
	{
		clock.restart();
		character++;
		displayTextStr.setString(scriptManager->getScriptLine().substr(0, character));
	}

	window.draw(displayBackground);
	window.draw(displayNameStr);
	window.draw(displayTextStr);
	
}

void MainState::update(float delta_t)
{
	displayNameStr.setString(scriptManager->getDisplayName());


	if (scriptManager->eof())
	{
		shouldChangeState = true;
		nextState = GameState::STATE_MENU;
	}

	GLOBAL->MAIN_STATE_currentFile = scriptManager->getCurrentFileName();
	GLOBAL->MAIN_STATE_currentLineId = scriptManager->getCurrentLineId();
}

void MainState::init()
{
	scriptManager->init();
	if (!background.loadFromFile(scriptManager->getBackgroundFileName()))
		LOGGER->Log("MenuState", "Unable to get Background Image");
	displayBackground.setTexture(background);

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
		displayTextStr.setFillColor(displayTextColor);
		displayTextStr.setCharacterSize(32);
		displayTextStr.setPosition(125.0f, 700.0f);
	}
}

const ScriptManager * MainState::getScriptManager()
{
	return scriptManager;
}

MainState::MainState(std::string filename, int lineId)
{
	scriptManager = new ScriptManager(filename, lineId);
}

MainState::~MainState()
{
	delete scriptManager;
}
