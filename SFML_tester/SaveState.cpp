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

void SaveState::writeSave(const std::string& savefile) const
{
	ofstream outfile(savefile, ios::binary | ios::out);
	if (!outfile)
	{
		LOGGER->Log("Save State", "Unable to create a new save file");
		return;
	}

	// create and read the screenshot
	screenshot.saveToFile("_temp_ss.png");
	ifstream ssfile("_temp_ss.png", ios::binary | ios::in);

	// create a string of that size
	std::string str;
	ssfile.seekg(ssfile.end);
	str.reserve(ssfile.tellg());
	ssfile.seekg(ssfile.beg);

	// read everything from the file
	str.assign(std::istreambuf_iterator<char>(ssfile), std::istreambuf_iterator<char>());
	unsigned int fileSize = str.length();
	outfile.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));
	outfile.write(reinterpret_cast<const char*>(str.c_str()), fileSize);

	// remove screenshot
	ssfile.close();
	std::remove("_temp_ss.png");

	std::string title = scriptManager->getDisplayName();
	if (title == "")
	{
		title = scriptManager->getScriptLine();
	}
	else
	{
		title = title + ": " + scriptManager->getScriptLine();
	}

	// save the title of the script
	UTILITY->writeToBinaryFile(outfile, title);

	// save the scriptManager
	scriptManager->serialize(outfile);

	outfile.close();
}

void SaveState::handleInput(sf::Event & e, sf::RenderWindow & window)
{

}

void SaveState::render(sf::RenderWindow & window)
{
	
}

void SaveState::update(float delta_t)
{

}

void SaveState::init()
{
	sf::Clock clock;
	writeSave("saves/savefile.dat");
	shouldChangeState = true;
	nextState = STATE_BACK;

	sf::Time time = clock.getElapsedTime();
	string msg = "Creating a save takes: " + to_string(time.asSeconds()) + "s";
	LOGGER->Log("SaveState", msg);
}

void SaveState::cleanup()
{
}
