#include "SaveState.h"

using namespace std;

SaveState::SaveState(const ScriptManager * scriptManager, sf::Image screenshot)
	:scriptManager(scriptManager), screenshot(screenshot)
{
	myState = STATE_SAVE;
	init();
}

SaveState::~SaveState()
{
}

void SaveState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	//qButton->handleInput(e, window);
	upArrow->handleInput(e, window);
	oneButton->handleInput(e, window);
	twoButton->handleInput(e, window);
	threeButton->handleInput(e, window);
	fourButton->handleInput(e, window);
	fiveButton->handleInput(e, window);
	sixButton->handleInput(e, window);
	sevenButton->handleInput(e, window);
	eightButton->handleInput(e, window);
	downArrow->handleInput(e, window);

	//CHECK STATE MANAGER
	for (MainButton* button : buttons)
	{
		button->handleInput(e, window);
	}

	if (menuButton->isClicked(true))
	{
		shouldChangeState = true;
		nextState = GameState::STATE_MENU;
		LOGGER->Log("SaveState", "Switching to Menu State");
	}
	if (returnButton->isClicked(true))
	{
		nextState = GameState::STATE_BACK;
		shouldChangeState = true;
		LOGGER->Log("SaveState", "Returning to Previous State");
	}
	if (quitButton->isClicked(true))
	{
		LOGGER->Log("SaveState", "Switching to Exit State");
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

			savefileImages[i]->resetDefault();
			SAVEDATAUTILITY->writeSave(savefile, screenshot, scriptManager);

			std::string title;
			sf::Image image;
			std::string savetime;

			if (UTILITY->checkFileExist(savefile))
			{
				SAVEDATAUTILITY->readSave(savefile, image, title, savetime);
				savefileImages[i]->setImage(image);
				savefileImages[i]->setTitle(title);
				savefileImages[i]->setDate(savetime);
			}

			return;
		}
	}

	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);

	switch (e.type)
	{
		case sf::Event::MouseButtonReleased:
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				if (returnState.getGlobalBounds().contains(mousePosF))
				{
					nextState = GameState::STATE_BACK;
					shouldChangeState = true;
					LOGGER->Log("SaveState", "Returning to prev state");
				}
			}
			else if (e.mouseButton.button == sf::Mouse::Right)
			{
				nextState = GameState::STATE_BACK;
				shouldChangeState = true;
				LOGGER->Log("SaveState", "Returning to prev state");
			}
			break;
		}

		case sf::Event::MouseMoved:
		{
			break;
		}
	}

	//Added this
	if (oneButton->isClicked(true))
	{
		currentPageNumber = 0;
		loadSavesByPage(currentPageNumber);
	}
	if (twoButton->isClicked(true))
	{
		currentPageNumber = 1;
		loadSavesByPage(currentPageNumber);
	}
	if (threeButton->isClicked(true))
	{
		currentPageNumber = 2;
		loadSavesByPage(currentPageNumber);
	}
	if (fourButton->isClicked(true))
	{
		currentPageNumber = 3;
		loadSavesByPage(currentPageNumber);
	}
	if (fiveButton->isClicked(true))
	{
		currentPageNumber = 4;
		loadSavesByPage(currentPageNumber);
	}
	if (sixButton->isClicked(true))
	{
		currentPageNumber = 5;
		loadSavesByPage(currentPageNumber);
	}
	if (sevenButton->isClicked(true))
	{
		currentPageNumber = 6;
		loadSavesByPage(currentPageNumber);
	}
	if (eightButton->isClicked(true))
	{
		currentPageNumber = 7;
		loadSavesByPage(currentPageNumber);
	}
	if (upArrow->isClicked(true))
	{
		if (currentPageNumber > 0)
		{
			currentPageNumber--;
			loadSavesByPage(currentPageNumber);
		}
		else
		{
			currentPageNumber = 7;
			loadSavesByPage(currentPageNumber);
		}
	}
	if (downArrow->isClicked(true))
	{
		if (currentPageNumber < 7)
		{
			currentPageNumber++;
			loadSavesByPage(currentPageNumber);
		}
		else
		{
			currentPageNumber = 0;
			loadSavesByPage(currentPageNumber);
		}
	}

	if (currentPageNumber == 0)
	{
		oneButton->highlight = true;
		twoButton->highlight = false;
		threeButton->highlight = false;
		fourButton->highlight = false;
		fiveButton->highlight = false;
		sixButton->highlight = false;
		sevenButton->highlight = false;
		eightButton->highlight = false;
	}
	else if (currentPageNumber == 1)
	{
		oneButton->highlight = false;
		twoButton->highlight = true;
		threeButton->highlight = false;
		fourButton->highlight = false;
		fiveButton->highlight = false;
		sixButton->highlight = false;
		sevenButton->highlight = false;
		eightButton->highlight = false;
	}
	else if (currentPageNumber == 2)
	{
		oneButton->highlight = false;
		twoButton->highlight = false;
		threeButton->highlight = true;
		fourButton->highlight = false;
		fiveButton->highlight = false;
		sixButton->highlight = false;
		sevenButton->highlight = false;
		eightButton->highlight = false;
	}
	else if (currentPageNumber == 3)
	{
		oneButton->highlight = false;
		twoButton->highlight = false;
		threeButton->highlight = false;
		fourButton->highlight = true;
		fiveButton->highlight = false;
		sixButton->highlight = false;
		sevenButton->highlight = false;
		eightButton->highlight = false;
	}
	else if (currentPageNumber == 4)
	{
		oneButton->highlight = false;
		twoButton->highlight = false;
		threeButton->highlight = false;
		fourButton->highlight = false;
		fiveButton->highlight = true;
		sixButton->highlight = false;
		sevenButton->highlight = false;
		eightButton->highlight = false;
	}
	else if (currentPageNumber == 5)
	{
		oneButton->highlight = false;
		twoButton->highlight = false;
		threeButton->highlight = false;
		fourButton->highlight = false;
		fiveButton->highlight = false;
		sixButton->highlight = true;
		sevenButton->highlight = false;
		eightButton->highlight = false;
	}
	else if (currentPageNumber == 6)
	{
		oneButton->highlight = false;
		twoButton->highlight = false;
		threeButton->highlight = false;
		fourButton->highlight = false;
		fiveButton->highlight = false;
		sixButton->highlight = false;
		sevenButton->highlight = true;
		eightButton->highlight = false;
	}
	else if (currentPageNumber == 7)
	{
		oneButton->highlight = false;
		twoButton->highlight = false;
		threeButton->highlight = false;
		fourButton->highlight = false;
		fiveButton->highlight = false;
		sixButton->highlight = false;
		sevenButton->highlight = false;
		eightButton->highlight = true;
	}
}

void SaveState::render(sf::RenderWindow & window)
{
	window.draw(saveBackground);
	//window.draw(returnState);

	for (int i = 0; i < savePerPage; i++)
	{
		window.draw(*(savefileImages[i]));
	}

	for (MainButton* button : buttons)
	{
		window.draw(*button);
	}
	//window.draw(*qButton);
	window.draw(*upArrow);
	window.draw(*oneButton);
	window.draw(*twoButton);
	window.draw(*threeButton);
	window.draw(*fourButton);
	window.draw(*fiveButton);
	window.draw(*sixButton);
	window.draw(*sevenButton);
	window.draw(*eightButton);
	window.draw(*downArrow);

	window.draw(rectangle);
}

void SaveState::update(float delta_t)
{
	//qButton->update(delta_t);
	upArrow->update(delta_t);
	oneButton->update(delta_t);
	twoButton->update(delta_t);
	threeButton->update(delta_t);
	fourButton->update(delta_t);
	fiveButton->update(delta_t);
	sixButton->update(delta_t);
	sevenButton->update(delta_t);
	eightButton->update(delta_t);
	downArrow->update(delta_t);

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

void SaveState::init()
{
	// initialize background stuffs
	if (!saveTexture.loadFromFile(GLOBAL->AssetRoot + "SavePage.png"))
	{
		LOGGER->Log("SaveState", "Image not found: SavePage.png");
	}
	saveTexture.setSmooth(true);
	saveBackground.setTexture(saveTexture);

	if (!settingsFont.loadFromFile(GLOBAL->UserInterfaceButtonFont))
	{
		LOGGER->Log("SaveState", "Unable to find default font");
	}

	//qButton = new DarkenButton(GLOBAL->AssetRoot + "Qbutton.png", "", "", 80.0f, 65.0f, 0, 0, 0, 0, 79, 73);
	//qButton->load();
	upArrow = new DarkenButton(GLOBAL->AssetRoot + "upArrowS.png", "", "", 80.0f, 159.0f, 0, 0, 0, 0, 79, 73);
	upArrow->load();
	oneButton = new DarkenButton(GLOBAL->AssetRoot + "1buttonS.png", "", "", 80.0f, 233.0f, 0, 0, 0, 0, 79, 73);
	oneButton->load();
	twoButton = new DarkenButton(GLOBAL->AssetRoot + "2buttonS.png", "", "", 80.0f, 307.0f, 0, 0, 0, 0, 79, 73);
	twoButton->load();
	threeButton = new DarkenButton(GLOBAL->AssetRoot + "3buttonS.png", "", "", 80.0f, 381.0f, 0, 0, 0, 0, 79, 73);
	threeButton->load();
	fourButton = new DarkenButton(GLOBAL->AssetRoot + "4buttonS.png", "", "", 80.0f, 455.0f, 0, 0, 0, 0, 79, 73);
	fourButton->load();
	fiveButton = new DarkenButton(GLOBAL->AssetRoot + "5buttonS.png", "", "", 80.0f, 529.0f, 0, 0, 0, 0, 79, 73);
	fiveButton->load();
	sixButton = new DarkenButton(GLOBAL->AssetRoot + "6buttonS.png", "", "", 80.0f, 603.0f, 0, 0, 0, 0, 79, 73);
	sixButton->load();
	sevenButton = new DarkenButton(GLOBAL->AssetRoot + "7buttonS.png", "", "", 80.0f, 677.0f, 0, 0, 0, 0, 79, 73);
	sevenButton->load();
	eightButton = new DarkenButton(GLOBAL->AssetRoot + "8buttonS.png", "", "", 80.0f, 751.0f, 0, 0, 0, 0, 79, 73);
	eightButton->load();
	downArrow = new DarkenButton(GLOBAL->AssetRoot + "downArrowS.png", "", "", 80.0f, 825.0f, 0, 0, 0, 0, 79, 73);
	downArrow->load();

	returnState.setFont(settingsFont);
	returnState.setString("Return");
	returnState.setFillColor(sf::Color::White);
	returnState.setCharacterSize(42);
	returnState.setPosition(880.0f, 790.0f);

	// create savefile images
	for (int i = 1; i <= savePerPage; i++)
	{
		savefileImages.push_back(new SavefileImage(i));
		savefileImages[i - 1]->load();
	}

	// load images and titles
	loadSavesByPage(0);

	savefileImages[INDEX_SAVE_1]->setPosition(180, 145);
	savefileImages[INDEX_SAVE_5]->setPosition(900, 145);
	savefileImages[INDEX_SAVE_2]->setPosition(180, 330);
	savefileImages[INDEX_SAVE_6]->setPosition(900, 330);
	savefileImages[INDEX_SAVE_3]->setPosition(180, 515);
	savefileImages[INDEX_SAVE_7]->setPosition(900, 515);
	savefileImages[INDEX_SAVE_4]->setPosition(180, 700);
	savefileImages[INDEX_SAVE_8]->setPosition(900, 700);

	menuButton = new MainButton(GLOBAL->AssetRoot + "Menu187x156S.png", "", "", 1210.0f, 101.0f, 0, 0, 0, 78, 187, 78, 0, 0, 187, 78);
	menuButton->load();
	buttons.push_back(menuButton);

	returnButton = new MainButton(GLOBAL->AssetRoot + "Return187x156S.png", "", "", 1000.0f, 101.0f, 0, 0, 0, 78, 187, 78, 0, 0, 187, 78);
	returnButton->load();
	buttons.push_back(returnButton);

	quitButton = new MainButton(GLOBAL->AssetRoot + "Quit187x156S.png", "", "", 1420.0f, 101.0f, 0, 0, 0, 78, 187, 78, 0, 0, 187, 78);
	quitButton->load();
	buttons.push_back(quitButton);

	currentAlpha = 0;
	endAlpha = 255;

	rectangle.setSize(sf::Vector2f(1600, 900));
	rectangle.setFillColor(sf::Color::Color(0, 0, 0, currentAlpha));
	rectangle.setPosition(0, 0);
	exitGame = false;
}

void SaveState::cleanup()
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

	//delete qButton;
	delete upArrow;
	delete oneButton;
	delete twoButton;
	delete threeButton;
	delete fourButton;
	delete fiveButton;
	delete sixButton;
	delete sevenButton;
	delete eightButton;
	delete downArrow;
}

void SaveState::loadSavesByPage(int pageNumber)
{
	// load the first page
	int currentSave = pageNumber * savePerPage;

	if (pageNumber < 0)
	{
		currentSave = 0;
	}

	for (int i = currentSave; i < currentSave + savePerPage; i++)
	{
		std::string savefile = SAVEDATAUTILITY->SavefileRoot + SAVEDATAUTILITY->SavefilePrefix + to_string(i) + SAVEDATAUTILITY->SavefileSuffix;
		if (pageNumber < 0)
		{
			savefile = SAVEDATAUTILITY->SavefileRoot + SAVEDATAUTILITY->QuickSavefilePrefix + to_string(i) + SAVEDATAUTILITY->QuickSavefileSuffix;
		}
		std::string title;
		sf::Image image;
		std::string savetime;

		if (UTILITY->checkFileExist(savefile))
		{
			SAVEDATAUTILITY->readSave(savefile, image, title, savetime);
			savefileImages[i - currentSave]->setImage(image);
			savefileImages[i - currentSave]->setTitle(title);
			savefileImages[i - currentSave]->setDate(savetime);
			savefileImages[i - currentSave]->resetDefault();
		}
		else
		{
			savefileImages[i - currentSave]->useDefaultSprite();
			savefileImages[i - currentSave]->setTitle("");
			savefileImages[i - currentSave]->setDate("");
		}
	}
}