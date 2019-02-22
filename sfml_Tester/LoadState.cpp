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
	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		if (returnState.getGlobalBounds().contains(mousePosF))
		{
			nextState = GameState::STATE_BACK;
			shouldChangeState = true;
			LOGGER->Log("SaveState", "Returning to Main Menu");
		}
		if (startNew.getGlobalBounds().contains(mousePosF))
		{
			shouldChangeState = true;
			//bgm.stop(); Music Still Plays
			nextState = GameState::STATE_NEW_GAME;
			LOGGER->Log("LoadState", "Starting a new game");
		}
		break;
	}
	}
}

void LoadState::render(sf::RenderWindow & window)
{
	window.draw(saveBackground);
	window.draw(startNew);
	window.draw(returnState);
}

void LoadState::update(float delta_t)
{

}

void LoadState::init()
{
	if (!saveTexture.loadFromFile("assets/LoadPage.png"))
		LOGGER->Log("MenuState", "Image not found: LoadPage.png");
	saveBackground.setTexture(saveTexture);

	if (!settingsFont.loadFromFile("assets/MATURASC.TTF"))
	{
		LOGGER->Log("SettingsState", "Unable to find default font");
	}

	startNew.setFont(settingsFont);
	startNew.setString("Start New Story");
	startNew.setFillColor(sf::Color::White);
	startNew.setCharacterSize(42);
	startNew.setPosition(380.0f, 790.0f);

	returnState.setFont(settingsFont);
	returnState.setString("Return to Menu");
	returnState.setFillColor(sf::Color::White);
	returnState.setCharacterSize(42);
	returnState.setPosition(880.0f, 790.0f);

	// FOR TEST PURPOSES ONLY!
	//sf::Image image;
	//std::string title;
	//ScriptManager* scriptManager;

	//sf::Clock clock;
	//string msg;
	//sf::Time time;

	//clock.restart();
	//readSave("saves/savefile.dat", image, title);
	//time = clock.getElapsedTime();
	//msg = "Loading a save w/o scriptmanager takes: " + to_string(time.asSeconds()) + "s";
	//LOGGER->Log("LoadState", msg);

	//clock.restart();
	//readSave("saves/savefile.dat", image, title, scriptManager);
	//time = clock.getElapsedTime();
	//msg = "Loading a save w/ scriptmanager takes: " + to_string( time.asSeconds() ) + "s";
	//LOGGER->Log("LoadState", msg);

	//delete scriptManager;
}

void LoadState::cleanup()
{
}

void LoadState::readSave(const std::string & savefile, sf::Image & image, std::string & title, ScriptManager *& scriptManager)
{
	ifstream infile(savefile, ios::binary | ios::in);
	if (!infile)
	{
		LOGGER->Log("LoadState", "Unable to load save file");
		return;
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
	image.saveToFile("saves/ss.png");

	// read the title
	title = UTILITY->readFromBinaryFile(infile);

	// read the script manager
	scriptManager = new ScriptManager(infile);
	infile.close();
}

void LoadState::readSave(const std::string & savefile, sf::Image & image, std::string & title)
{
	ifstream infile(savefile, ios::binary | ios::in);
	if (!infile)
	{
		LOGGER->Log("Save State", "Unable to load save file");
		return;
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
	image.saveToFile("saves/ss.png");

	// read the title
	title = UTILITY->readFromBinaryFile(infile);
	infile.close();
}
