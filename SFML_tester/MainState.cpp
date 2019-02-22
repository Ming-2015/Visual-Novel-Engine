#include "MainState.h"
#include "Config.h"

void MainState::handleInput(sf::Event & e, sf::RenderWindow & window)
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
}

void MainState::cleanup()
{
	if (scriptManager) delete scriptManager;
}

const ScriptManager * MainState::getScriptManager()
{
	return scriptManager;
}

MainState::MainState()
{
	scriptManager = new ScriptManager(GLOBAL->NewGameScriptFileLocation);
	myState = GameState::STATE_MAIN;
	init();
}

MainState::MainState(std::string savefile)
{
	ifstream ifile(savefile, ios::binary | ios::in);
	if (ifile)
	{
		scriptManager = new ScriptManager(ifile);
		ifile.close();
	}
	else
	{
		scriptManager = new ScriptManager(GLOBAL->NewGameScriptFileLocation);
	}

	myState = GameState::STATE_MAIN;
	init();
}

MainState::~MainState()
{
}
