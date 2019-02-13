#include "SaveState.h"
#include <iostream>
#include <fstream>
using namespace std;

void SaveState::writeSave() {
	ofstream myfile("Save.txt");
	if (myfile.is_open())
	{
		myfile << "File Name =" << scriptFileName << '\n';		//Change?
		myfile << "File line ID = " << lineId << '\n';	//Change?
		myfile.close();
	}
	else {
		string err = "Incorrect Save file";
		LOGGER->Log("SaveFile", err);
	}
}

void SaveState::readSave() {
	string line;
	ifstream thefile("testSave.txt");
	if (thefile.is_open())
	{
		stringstream ss(line);
		getline(ss, tempStr, '=');
		getline(ss, fileNameFromSave, '=');
		getline(ss, tempStr, '=');
		getline(ss, tempStr, '=');
		lineIDFromSave = stoi(tempStr);
	}
	else {
		string err = "Incorrect Save file";
		LOGGER->Log("SaveFile", err);
	}
}

void SaveState::handleInput(sf::Event & e, sf::RenderWindow & window)
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
			}
			if (startNew.getGlobalBounds().contains(mousePosF))
			{
				shouldChangeState = true;
				//bgm.stop(); Music Still Plays
				nextState = GameState::STATE_NEW_GAME;
				LOGGER->Log("SaveState", "Starting a new game");
			}
			break;
		}
	}
}

void SaveState::render(sf::RenderWindow & window)
{
	window.draw(saveBackground);
	window.draw(startNew);
	window.draw(returnState);
}

void SaveState::update(float delta_t)
{

}

void SaveState::init()
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
}
