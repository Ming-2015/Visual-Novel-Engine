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
	for (int i = 0; i < savePerPage; i++)
	{
		savefileImages[i]->handleInput(e, window);
	}

	for (int i = 0; i < savePerPage; i++)
	{
		if (savefileImages[i]->isClicked(true))
		{
			std::string savefile = GLOBAL->SavefileRoot + GLOBAL->SavefilePrefix +
				to_string(currentPageNumber*savePerPage + i) + GLOBAL->SavefileSuffix;

			sf::Image img;
			std::string str;
			if (readSave(savefile, img, str, scriptManager))
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
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
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
	window.draw(startNew);
	window.draw(returnState);

	for (int i = 0; i < savePerPage; i++)
	{
		window.draw(*(savefileImages[i]) );
	}
}

void LoadState::update(float delta_t)
{
	for (int i = 0; i < savePerPage; i++)
	{
		savefileImages[i]->update(delta_t);
	}
}

void LoadState::init()
{
	if (!saveTexture.loadFromFile(GLOBAL->AssetRoot + "LoadPage.png"))
	{
		LOGGER->Log("LoadState", "Image not found: LoadPage.png");
	}
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

void LoadState::cleanup()
{
	for (SavefileImage* save : savefileImages)
	{
		if (save != nullptr) delete save;
	}
}

bool LoadState::readSave(const std::string & savefile, sf::Image & image, std::string & title, ScriptManager *& scriptManager)
{
	ifstream infile(savefile, ios::binary | ios::in);
	if (!infile)
	{
		LOGGER->Log("LoadState", "Unable to load save file");
		return false;
	}

	// read the image file size
	unsigned int fileSize;
	infile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

	// read the image data from file
	std::vector<char> byteArray(fileSize);
	infile.read(byteArray.data(), fileSize);

	// dump it into a memory input stream
	sf::MemoryInputStream picStream;
	picStream.open(byteArray.data(), fileSize);

	// load the image!
	image.loadFromStream(picStream);

	// read the title
	title = UTILITY->readFromBinaryFile(infile);

	// read the script manager
	scriptManager = new ScriptManager(infile);
	infile.close();

	return true;
}

bool LoadState::readSave(const std::string & savefile, sf::Image & image, std::string & title)
{
	ifstream infile(savefile, ios::binary | ios::in);
	if (!infile)
	{
		LOGGER->Log("LoadState", "Unable to load save file");
		return false;
	}

	// read the image file size
	unsigned int fileSize;
	infile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

	// read the image data from file
	std::vector<char> byteArray(fileSize);
	infile.read(byteArray.data(), fileSize);

	// dump it into a memory input stream
	sf::MemoryInputStream picStream;
	picStream.open(byteArray.data(), fileSize);

	// load the image!
	image.loadFromStream(picStream);

	// read the title
	title = UTILITY->readFromBinaryFile(infile);
	infile.close();

	return true;
}

void LoadState::loadSavesByPage(int pageNumber)
{
	// load the first page
	int currentSave = pageNumber * savePerPage;
	for (int i = currentSave; i < currentSave + savePerPage; i++)
	{
		std::string savefile = GLOBAL->SavefileRoot + GLOBAL->SavefilePrefix + to_string(i) + GLOBAL->SavefileSuffix;
		std::string title;
		sf::Image image;

		if (UTILITY->checkFileExist(savefile))
		{
			readSave(savefile, image, title);
			savefileImages[i - currentSave]->setImage(image);
			savefileImages[i - currentSave]->setString(title);
		}
		else
		{
			savefileImages[i - currentSave]->useDefaultSprite();
		}
	}
}
