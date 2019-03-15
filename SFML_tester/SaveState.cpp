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

			SAVEDATAUTILITY->writeSave(savefile, screenshot, scriptManager);

			std::string title;
			sf::Image image;
			std::string savetime;

			if (UTILITY->checkFileExist(savefile))
			{
				SAVEDATAUTILITY->readSave(savefile, image, title, savetime);
				savefileImages[i]->setImage(image);
				savefileImages[i]->setString(title);
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
}

void SaveState::render(sf::RenderWindow & window)
{
	window.draw(saveBackground);
	window.draw(returnState);

	for (int i = 0; i < savePerPage; i++)
	{
		window.draw(*(savefileImages[i]));
	}
}

void SaveState::update(float delta_t)
{
	for (int i = 0; i < savePerPage; i++)
	{
		savefileImages[i]->update(delta_t);
	}
}

void SaveState::init()
{
	// initialize background stuffs
	if (!saveTexture.loadFromFile(GLOBAL->AssetRoot + "LoadPage.png"))
	{
		LOGGER->Log("SaveState", "Image not found: LoadPage.png");
	}
	saveTexture.setSmooth(true);
	saveBackground.setTexture(saveTexture);

	if (!settingsFont.loadFromFile(GLOBAL->UserInterfaceButtonFont))
	{
		LOGGER->Log("SaveState", "Unable to find default font");
	}

	returnState.setFont(settingsFont);
	returnState.setString("Return");
	returnState.setFillColor(sf::Color::White);
	returnState.setCharacterSize(42);
	returnState.setPosition(880.0f, 790.0f);

	// create savefile images
	for (int i = 0; i < savePerPage; i++)
	{
		savefileImages.push_back(new SavefileImage());
		savefileImages[i]->load();
	}

	// load images and titles
	loadSavesByPage(0);

	savefileImages[INDEX_SAVE_1]->setPosition(170, 260);
	savefileImages[INDEX_SAVE_2]->setPosition(613, 257);
	savefileImages[INDEX_SAVE_3]->setPosition(1069, 256);
	savefileImages[INDEX_SAVE_4]->setPosition(161, 521);
	savefileImages[INDEX_SAVE_5]->setPosition(614, 519);
	savefileImages[INDEX_SAVE_6]->setPosition(1070, 520);
}

void SaveState::cleanup()
{
	for (SavefileImage* save : savefileImages)
	{
		if (save != nullptr) delete save;
	}
}

void SaveState::loadSavesByPage(int pageNumber)
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
			savefileImages[i - currentSave]->setString(title);
		}
		else
		{
			savefileImages[i - currentSave]->useDefaultSprite();
		}
	}
}