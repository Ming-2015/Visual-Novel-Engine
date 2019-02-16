#include "MainState.h"
#include "Config.h"

void MainState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	scriptManager->handleInput(e, window);
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

	if (!scriptManager->shouldHideTextbox())
	{
		window.draw(*(scriptManager->getTextboxImage()));
	}
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
