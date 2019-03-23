#include "MainState.h"
#include "Config.h"
#include "GameState.h"
#include "scriptLine.h"

void MainState::handleInput(sf::Event & e, sf::RenderWindow & window)
{	
	if (!returnMenuPrompt->shouldBeHidden())
	{
		returnMenuPrompt->handleInput(e, window);
	}
	// only update the buttons if you don't need to draw
	else if (!scriptManager->shouldHideTextbox())
	{
		drawMainButton->handleInput(e, window);
	}

	// stop skipMode if user presses a button
	if (GLOBAL->skipMode)
	{
		switch (e.type)
		{
			case sf::Event::KeyReleased:
			{
				if (e.key.code == sf::Keyboard::Enter)
				{
					GLOBAL->skipMode = false;
				}	
				break;
			}
		}
	}

	if (GLOBAL->autoMode)
	{
		switch (e.type)
		{
			case sf::Event::KeyReleased:
			{
				if (e.key.code == sf::Keyboard::Enter)
				{
					GLOBAL->autoMode = false;
				}					
				break;
			}
		}
	}

	if (!returnMenuPrompt->shouldBeHidden())
	{
		if (returnMenuPrompt->isNoButtonClicked(true))
		{
			returnMenuPrompt->hidePrompt(true);
		}
		else if (returnMenuPrompt->isYesButtonClicked(true))
		{
			shouldChangeState = true;
			////bgm.stop();
			nextState = GameState::STATE_MENU;
			LOGGER->Log("MenuState", "Switching to Menu State");
			drawMainButton->exitButtonClicked = false;
		}
	}
	else if (scriptManager->getLineLog()->shouldDisplay())
	{
		scriptManager->getLineLog()->handleInput(e, window);
	}
	else if ( scriptManager->isTextboxClosed() )
	{
		switch (e.type)
		{
			case sf::Event::KeyReleased:
			{
				if (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::H )
				{
					scriptManager->showTextbox();
				}					
				break;
			}

			case sf::Event::MouseButtonReleased:
			{
				if (GLOBAL->windowPtr->hasFocus())
				{
					scriptManager->showTextbox();
				}
				break;
			}
		}
	}
	else if (drawMainButton->logButtonClicked == true)
	{
		scriptManager->getLineLog()->displayLog(true);
		drawMainButton->logButtonClicked = false;
	}
	else if (drawMainButton->configButtonClicked == true)
	{

		shouldChangeState = true;
		nextState = GameState::STATE_CONFIG;
		LOGGER->Log("MenuState", "Opening settings");
		drawMainButton->configButtonClicked = false;

	}
	else if (drawMainButton->exitButtonClicked == true)
	{
		returnMenuPrompt->hidePrompt(false);
		drawMainButton->exitButtonClicked = false;
	}
	else if (drawMainButton->quickLoadButtonClicked == true)
	{
		//quick load options
		drawMainButton->quickLoadButtonClicked = false;

		int saveId = 1;
		std::string fname = SAVEDATAUTILITY->SavefileRoot +
			SAVEDATAUTILITY->QuickSavefilePrefix + to_string(saveId) +
			SAVEDATAUTILITY->QuickSavefileSuffix;

		sf::Image image;
		std::string title;
		std::string savetime;

		delete scriptManager;
		if (UTILITY->checkFileExist(fname) && 
			!SAVEDATAUTILITY->readSave(fname, image, title, savetime, scriptManager))
		{
			LOGGER->Log("MainState", "Failed to load quick save file");
		}
	}
	else if (drawMainButton->loadButtonClicked == true)
	{
		//load options
		drawMainButton->loadButtonClicked = false;
		shouldChangeState = true;
		nextState = STATE_LOAD;	
	}
	else if (drawMainButton->quickSaveButtonClicked == true)
	{
		//quickSave options
		drawMainButton->quickSaveButtonClicked = false;

		int saveId = 6;
		std::string fname = SAVEDATAUTILITY->SavefileRoot +
			SAVEDATAUTILITY->QuickSavefilePrefix + to_string(saveId) +
			SAVEDATAUTILITY->QuickSavefileSuffix;
		if (UTILITY->checkFileExist(fname) && !remove(fname.c_str()))
		{
			LOGGER->Log("MainState", "Unable to delete past quick save data");
		}
		
		saveId--;
		std::string newfname;
		for (; saveId >= 1; saveId--)
		{
			fname = SAVEDATAUTILITY->SavefileRoot +
				SAVEDATAUTILITY->QuickSavefilePrefix + to_string(saveId) +
				SAVEDATAUTILITY->QuickSavefileSuffix;
			newfname = SAVEDATAUTILITY->SavefileRoot +
				SAVEDATAUTILITY->QuickSavefilePrefix + to_string(saveId+1) +
				SAVEDATAUTILITY->QuickSavefileSuffix;

			if (UTILITY->checkFileExist(fname) && !rename(fname.c_str(), newfname.c_str()))
			{
				LOGGER->Log("MainState", "Unable to rename quick save data");
			}
		}

		// for some reason this is necessary
		const ScriptManager* copy = scriptManager;

		SAVEDATAUTILITY->writeSave(fname, 
			UTILITY->getScreenshot(window, 5, 5), copy);
	}
	else if (drawMainButton->saveButtonClicked == true )
	{
		//save options
		drawMainButton->saveButtonClicked = false;
		shouldChangeState = true;
		nextState = STATE_SAVE;
		GLOBAL->scriptManagerPtr = scriptManager;
	}
	else if (drawMainButton->autoButtonClicked == true )
	{
		GLOBAL->autoMode = !GLOBAL->autoMode;
		GLOBAL->skipMode = false;
		drawMainButton->autoButtonClicked = false;
	}

	else if (drawMainButton->skipButtonClicked == true)
	{
		GLOBAL->skipMode = !GLOBAL->skipMode;
		GLOBAL->autoMode = false;
		drawMainButton->skipButtonClicked = false;
	}
	else if (drawMainButton->closeButtonClicked == true)
	{
		//What to do if close button is clicked
		drawMainButton->closeButtonClicked = false;
		scriptManager->hideTextbox();
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
					return;
				}
				else if (e.key.code == sf::Keyboard::A)
				{
					GLOBAL->autoMode = !GLOBAL->autoMode;
					GLOBAL->skipMode = false;
					return;
				}
				else if (e.key.code == sf::Keyboard::Escape)
				{
					returnMenuPrompt->hidePrompt(false);
					return;
				}
				else if (e.key.code == sf::Keyboard::H)
				{
					scriptManager->hideTextbox();
					return;
				}

				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				if (e.mouseButton.button == sf::Mouse::Right)
				{
					scriptManager->hideTextbox();
					return;
				}
				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				if (e.mouseWheelScroll.delta > 0)
				{
					scriptManager->getLineLog()->displayLog(true);
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

	if (scriptManager->getCinematicBars())
	{
		scriptManager->getCinematicBars()->render(window);
	}

	if (scriptManager->getLineLog()->shouldDisplay())
	{
		scriptManager->getLineLog()->render(window);
	}
	else
	{
		if (!scriptManager->shouldHideTextbox())
		{
			drawMainButton->setAlpha(scriptManager->getTextboxImage()->getFontAlpha());
			window.draw(*(scriptManager->getTextboxImage()));
			drawMainButton->render(window);
		}

		if (scriptManager->isChoice() && !scriptManager->shouldHideTextbox())
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

	if (!returnMenuPrompt->shouldBeHidden())
	{
		returnMenuPrompt->render(window);
	}
}

void MainState::update(float delta_t)
{
	if (scriptManager->getLineLog()->shouldDisplay())
	{
		scriptManager->getLineLog()->update(delta_t);
	}
	else
	{
		drawMainButton->update(delta_t);
		returnMenuPrompt->update(delta_t);

		// update scriptmanager 
		scriptManager->update(delta_t);

		// return if reached end of script (unexpectedly)
		if (scriptManager->eof())
		{
			shouldChangeState = true;
			nextState = GameState::STATE_MENU;
		}
	}

	drawMainButton->highlightAutoButton(GLOBAL->autoMode);
	drawMainButton->highlightSkipButton(GLOBAL->skipMode || GLOBAL->ctrlSkipMode);
}

void MainState::init()
{
	if (GLOBAL->scriptManagerPtr != nullptr)
	{
		scriptManager = GLOBAL->scriptManagerPtr;
		GLOBAL->scriptManagerPtr = nullptr;
	}
	else
	{
		scriptManager = new ScriptManager(GLOBAL->NewGameScriptFileLocation);
	}

	GLOBAL->playerName = scriptManager->getPlayerName();

	GLOBAL->skipMode = false;
	GLOBAL->autoMode = false;
	if (drawMainButton == nullptr)
		drawMainButton = new DrawMainButton();
	if (returnMenuPrompt == nullptr)
		returnMenuPrompt = new ConfirmationPrompt("Return To Menu?", "mainToMenu");	
}

void MainState::cleanup()
{
	if (scriptManager) delete scriptManager;
	if (drawMainButton) delete drawMainButton;
	if (returnMenuPrompt) delete returnMenuPrompt;
}

const ScriptManager * MainState::getScriptManager()
{
	return scriptManager;
}

MainState::MainState()
{
	myState = GameState::STATE_MAIN;
	init();
}

MainState::MainState(std::string playerName)
{
	scriptManager = new ScriptManager(GLOBAL->NewGameScriptFileLocation);
	scriptManager->setPlayerName(playerName);
	init();

	GLOBAL->playerName = scriptManager->getPlayerName();
}

MainState::~MainState()
{
	cleanup();
}
