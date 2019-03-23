#include "LoadState.h"

LoadState::LoadState()
{
	myState = GameState::STATE_LOAD;
	init();
}

LoadState::~LoadState()
{

}

void LoadState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	for (MainButton* button : buttons)
	{
		button->handleInput(e, window);
	}

	if (menuButton->isClicked(true))
	{
		shouldChangeState = true;
		nextState = GameState::STATE_MENU;
		LOGGER->Log("LoadState", "Switching to Menu State");
	}
	if (returnButton->isClicked(true))
	{
		nextState = GameState::STATE_BACK;
		shouldChangeState = true;
		LOGGER->Log("LoadState", "Returning to Previous State");
	}
	if (quitButton->isClicked(true))
	{
		LOGGER->Log("MenuState", "Switching to Exit State");
		exitGame = true;
	}

	for (int i = 0; i < savePerPage; i++)
	{
		savefileImages[i]->handleInput(e, window);
	}

	for (int i = 0; i < savePerPage; i++)
	{
		if (savefileImages[i]->isClicked(true))
		{
			std::string savefile = SAVEDATAUTILITY->SavefileRoot + SAVEDATAUTILITY->SavefilePrefix +
				to_string(currentPageNumber*savePerPage + i) + SAVEDATAUTILITY->SavefileSuffix;

			sf::Image img;
			std::string str;
			std::string savetime;
			if (SAVEDATAUTILITY->readSave(savefile, img, str, savetime, scriptManager))
			{
				GLOBAL->scriptManagerPtr = scriptManager;

				nextState = GameState::STATE_MAIN;
				shouldChangeState = true;
				LOGGER->Log("LoadState", "Loading a game and entering main state");
				return;
			}
			else
			{
				std::string msg = "save file: " + savefile + " does not exist!";
				LOGGER->Log("LoadState", msg);
			}
		}
	}

	switch (e.type)
	{
		case sf::Event::MouseButtonReleased:
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
				if (returnState.getGlobalBounds().contains(mousePosF))
				{
					nextState = GameState::STATE_BACK;
					shouldChangeState = true;
					LOGGER->Log("LoadState", "Returning to prev state");
				}
				if (startNew.getGlobalBounds().contains(mousePosF))
				{
					shouldChangeState = true;
					nextState = GameState::STATE_NEW_GAME;
					LOGGER->Log("LoadState", "Starting a new game");
				}
			}
			else if (e.mouseButton.button == sf::Mouse::Right)
			{
				nextState = GameState::STATE_BACK;
				shouldChangeState = true;
				LOGGER->Log("LoadState", "Returning to prev state");
			}
			break;
		}

		case sf::Event::MouseMoved:
		{
			break;
		}
	}
}

void LoadState::render(sf::RenderWindow & window)
{
	window.draw(saveBackground);
	//window.draw(startNew);
	//window.draw(returnState);

	for (int i = 0; i < savePerPage; i++)
	{
		window.draw(*(savefileImages[i]) );
	}

	for (MainButton* button : buttons)
	{
		window.draw(*button);
	}
	
	window.draw(rectangle);
}

void LoadState::update(float delta_t)
{
	for (int i = 0; i < savePerPage; i++)
	{
		savefileImages[i]->update(delta_t);
	}

	for (MainButton* button : buttons)
	{
		button->update(delta_t);
	}

	if (exitGame == true)
	{
		if (clock.getElapsedTime().asMilliseconds() > 20.0f)
		{
			if (currentAlpha < endAlpha) {
				currentAlpha += 10;

				if (currentAlpha >= endAlpha)
				{
					currentAlpha = endAlpha;
				}
				rectangle.setFillColor(sf::Color::Color(0, 0, 0, currentAlpha));
			}
			else
			{
				shouldChangeState = true;
				nextState = GameState::STATE_EXIT;
			}
			clock.restart();
		}
	}
}

void LoadState::init()
{
	if (!saveTexture.loadFromFile(GLOBAL->AssetRoot + "LoadPage.png"))
	{
		LOGGER->Log("LoadState", "Image not found: LoadPage.png");
	}
	saveTexture.setSmooth(true);
	saveBackground.setTexture(saveTexture);

	if (!settingsFont.loadFromFile(GLOBAL->UserInterfaceButtonFont))
	{
		LOGGER->Log("LoadState", "Unable to find default font");
	}

	startNew.setFont(settingsFont);
	startNew.setString("Start New Story");
	startNew.setFillColor(sf::Color::White);
	startNew.setCharacterSize(42);
	startNew.setPosition(380.0f, 790.0f);

	returnState.setFont(settingsFont);
	returnState.setString("Return");
	returnState.setFillColor(sf::Color::White);
	returnState.setCharacterSize(42);
	returnState.setPosition(880.0f, 790.0f);

	// create savefile images
	for (int i = 1; i <= savePerPage; i++)
	{
		savefileImages.push_back(new SavefileImage(i));
		savefileImages[i-1]->load();
	}

	// load images and titles
	loadSavesByPage(0);

	savefileImages[INDEX_SAVE_1]->setPosition(170, 140);
	savefileImages[INDEX_SAVE_5]->setPosition(870, 140);
	savefileImages[INDEX_SAVE_2]->setPosition(170, 330);
	savefileImages[INDEX_SAVE_6]->setPosition(870, 330);
	savefileImages[INDEX_SAVE_3]->setPosition(170, 520);
	savefileImages[INDEX_SAVE_7]->setPosition(870, 520);
	savefileImages[INDEX_SAVE_4]->setPosition(170, 700);
	savefileImages[INDEX_SAVE_8]->setPosition(870, 700);

	menuButton = new MainButton(GLOBAL->AssetRoot + "Menu187x156.png", "", "", 1210.0f, 97.0f, 0, 0, 0, 78, 187, 78, 0, 0, 187, 78);
	menuButton->load();
	buttons.push_back(menuButton);

	returnButton = new MainButton(GLOBAL->AssetRoot + "Return187x156.png", "", "", 1000.0f, 97.0f, 0, 0, 0, 78, 187, 78, 0, 0, 187, 78);
	returnButton->load();
	buttons.push_back(returnButton);

	quitButton = new MainButton(GLOBAL->AssetRoot + "Quit187x156.png", "", "", 1420.0f, 97.0f, 0, 0, 0, 78, 187, 78, 0, 0, 187, 78);
	quitButton->load();
	buttons.push_back(quitButton);

	currentAlpha = 0;
	endAlpha = 255;

	rectangle.setSize(sf::Vector2f(1600, 900));
	rectangle.setFillColor(sf::Color::Color(0, 0, 0, currentAlpha));
	rectangle.setPosition(0, 0);
	exitGame = false;
}

void LoadState::cleanup()
{
	for (SavefileImage* save : savefileImages)
	{
		if (save != nullptr) delete save;
	}
	for (MainButton* button : buttons)
	{
		delete button;
	}
	buttons.clear();
}

void LoadState::loadSavesByPage(int pageNumber)
{
	// load the first page
	int currentSave = pageNumber * savePerPage;
	for (int i = currentSave; i < currentSave + savePerPage; i++)
	{
		std::string savefile = SAVEDATAUTILITY->SavefileRoot + SAVEDATAUTILITY->SavefilePrefix + to_string(i) + SAVEDATAUTILITY->SavefileSuffix;
		std::string title;
		sf::Image image;
		std::string savetime;

		if (UTILITY->checkFileExist(savefile))
		{
			SAVEDATAUTILITY->readSave(savefile, image, title, savetime);
			savefileImages[i - currentSave]->setImage(image);
			savefileImages[i - currentSave]->setTitle(title);
			savefileImages[i - currentSave]->setDate(savetime);
		}
		else
		{
			savefileImages[i - currentSave]->useDefaultSprite();
		}
	}
}
