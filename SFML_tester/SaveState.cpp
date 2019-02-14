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

}

void SaveState::render(sf::RenderWindow & window)
{
	
}

void SaveState::update(float delta_t)
{

}

void SaveState::init()
{
	
}

void SaveState::cleanup()
{
}
