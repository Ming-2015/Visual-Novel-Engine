#include "SaveState.h"
#include <iostream>
#include <fstream>
using namespace std;

SaveState::SaveState() {
	
}

void SaveState::writeSave() {
	ofstream myfile("Save.txt");
	if (myfile.is_open())
	{
		myfile << "File Name =" << scriptManager->getCurrentFileName() << '\n';		//Change?
		myfile << "File line ID = " << scriptManager->getCurrentLineId() << '\n';	//Change?
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
		stringstream ss(line);
		getline(ss, tempStr, '=');
		getline(ss, tempStr, '=');
		lineIDFromSave = stoi(tempStr);
	}
	else {
		string err = "Incorrect Save file";
		LOGGER->Log("SaveFile", err);
	}
}