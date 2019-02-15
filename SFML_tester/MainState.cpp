#include "MainState.h"
#include "Config.h"

void MainState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	switch (e.type)
	{
		case sf::Event::MouseButtonPressed:
		{

		}
		break;
	}
}

void MainState::render(sf::RenderWindow & window)
{
	for (BackgroundImage* bg : scriptManager->getBackgroundImages())
	{
		window.draw(*bg);
	}

	for (CharacterImage* character : scriptManager->getCharacterImages())
	{
		window.draw(*character);
	}

	window.draw(displayNameStr);
	window.draw(displayTextStr);
	
}

void MainState::update(float delta_t)
{
	//LOGGER->Log("MainState", "Updating script manager");
	if (scriptManager->doneAllCommands())
	{
		LOGGER->Log("MainState", "Reading new command");
		scriptManager->readCommands();
	}
	scriptManager->update(delta_t);

	displayTextStr.setString(scriptManager->getScriptLine());
	displayNameStr.setString(scriptManager->getDisplayName());

	if (scriptManager->eof())
	{
		shouldChangeState = true;
		nextState = GameState::STATE_MENU;
	}

	GLOBAL->MAIN_STATE_currentFile = scriptManager->getCurrentFileName();
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
		displayNameStr.setPosition(230.0f, 643.0f);
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
		displayTextStr.setPosition(230.0f, 705.0f);
	}
}

void MainState::cleanup()
{
}

const ScriptManager * MainState::getScriptManager()
{
	return scriptManager;
}

MainState::MainState(std::string filename)
{
	scriptManager = new ScriptManager(filename);
	myState = GameState::STATE_MAIN;
	init();
}

MainState::~MainState()
{
	if (scriptManager) delete scriptManager;
}
