#include "MainState.h"
#include "Config.h"
#include "GameState.h"
#include "scriptLine.h"

void MainState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	drawMainButton->handleInput(e, window);
	
	if (drawMainButton->configButtonClicked == true)
	{
		shouldChangeState = true;
		nextState = GameState::STATE_CONFIG;
		LOGGER->Log("MenuState", "Opening settings");
		drawMainButton->configButtonClicked = false;
		//DOES NO SWITCH TO SETTINGS PAGE
	}
	else if (drawMainButton->exitButtonClicked == true)
	{
		shouldChangeState = true;
		//bgm.stop();
		nextState = GameState::STATE_EXIT;
		LOGGER->Log("MenuState", "Switching to Exit State");
		drawMainButton->exitButtonClicked = false;
		//Does not exit
	}
	else if (drawMainButton->quickLoadButtonClicked == true)
	{
		//quick load options
		drawMainButton->quickLoadButtonClicked = false;
	}
	else if (drawMainButton->loadButtonClicked == true)
	{
		//load options
		drawMainButton->loadButtonClicked = false;
	}
	else if (drawMainButton->quickSaveButtonClicked == true)
	{
		//quickSave options
		drawMainButton->quickSaveButtonClicked = false;
	}
	else if (drawMainButton->saveButtonClicked == true)
	{
		//save options
		drawMainButton->saveButtonClicked = false;
	}
	else if (drawMainButton->autoButtonClicked == true)
	{
		//auto options
		drawMainButton->autoButtonClicked = false;
	}
	else if (drawMainButton->skipButtonClicked == true)
	{
		//skip button options
		drawMainButton->skipButtonClicked = false;
	}
	else
	{
		switch (e.type)
		{
		case sf::Event::KeyReleased:
		{
			if (e.key.code == sf::Keyboard::S)
			{
				shouldChangeState = true;
				nextState = STATE_SAVE;
				GLOBAL->scriptManagerPtr = scriptManager;
				return;
			}
			else if (e.key.code == sf::Keyboard::L)
			{
				shouldChangeState = true;
				nextState = STATE_LOAD;
				GLOBAL->scriptManagerPtr = scriptManager;
				return;
			}

			break;
		}
		}

		// handle script-related input
		scriptManager->handleInput(e, window);
	}
}

void MainState::render(sf::RenderWindow & window)
{
	for (BackgroundImage* bg : scriptManager->getBackgroundImages())
	{
		if (bg->shouldUseShader() == true)
		{
			window.draw(*bg, bg->getShader());
		}
		else
		{
			window.draw(*bg);
		}
	}

	for (CharacterImage* character : scriptManager->getCharacterImages())
	{
		if (character->shouldUseShader() == true)
		{
			window.draw(*character, character->getShader());
		}
		else
		{
			window.draw(*character);
		}
	}

	if (!scriptManager->shouldHideTextbox())
	{
		window.draw(*(scriptManager->getTextboxImage()));
		drawMainButton->render(window);
	}
	
	if (scriptManager->isChoice())
	{
		for (auto c : scriptManager->getChoices())
		{
			if (c != nullptr)
			{
				window.draw(*c);
			}
		}
	}
}

void MainState::update(float delta_t)
{
	drawMainButton->update(delta_t);
	//LOGGER->Log("MainState", "Updating script manager");
	if (scriptManager->doneAllCommands())
	{
		scriptManager->readCommands();
	}
	scriptManager->update(delta_t);

	if (scriptManager->eof())
	{
		shouldChangeState = true;
		nextState = GameState::STATE_MENU;
	}

}

void MainState::init()
{
	scriptManager->init();
	GLOBAL->skipMode = false;
	GLOBAL->autoMode = false;
	drawMainButton = new DrawMainButton();

}

void MainState::cleanup()
{
	if (scriptManager) delete scriptManager;
	if (drawMainButton) delete drawMainButton;
}

const ScriptManager * MainState::getScriptManager()
{
	return scriptManager;
}

MainState::MainState()
{
	if (GLOBAL->scriptManagerPtr != nullptr)
	{
		scriptManager = GLOBAL->scriptManagerPtr;
		GLOBAL->scriptManagerPtr = nullptr;
	}
	else
	{
		scriptManager = new ScriptManager(GLOBAL->selectedLoadFile);
	}
	myState = GameState::STATE_MAIN;
	init();
}

MainState::MainState(std::string playerName)
{
	scriptManager = new ScriptManager(GLOBAL->NewGameScriptFileLocation);
	scriptManager->setPlayerName(playerName);
	myState = GameState::STATE_MAIN;
	init();
}

MainState::~MainState()
{
}
