#include "Config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <sstream>
#include "Utility.h"
using namespace std;

Config::Config()
{
	windowWidth = 1600;
	windowHeight = 900;
	windowTitle = "Visual Novel Engine";
	fps = 60;
}

void Config::write() {

	Utility util;

	ofstream myfile("config.txt");
	if (myfile.is_open())
	{
		myfile << "masterVolume:" << '\t' << to_string(masterVolume) << endl;
		myfile << "bgmVolume:" << '\t' << to_string(bgmVolume) << endl;
		myfile << "voiceVolume:" << '\t' << to_string(voiceVolume) << endl;
		myfile << "sfxVolume:" << '\t' << to_string(sfxVolume) << endl;
		myfile << "fullScreen:" << '\t' << to_string(fullScreen) << endl;
		myfile << "manualTextSpeed:" << '\t' << to_string(manualTextSpeed) << endl;
		myfile << "autoTextSpeed:" << '\t' << to_string(autoTextSpeed) << endl;
		myfile << "autoTextWaitTime:" << '\t' << to_string(autoTextWaitTime) << endl;
		myfile << "textFade:" << '\t' << util.bool2str(textFade) << endl;
		myfile << "skipUnreadText:" << '\t' << util.bool2str(skipUnreadText) << endl;
		myfile << "fontFileName:" << '\t' << fontFileName << endl;
		myfile << "textWindowTransparency:" << '\t' << to_string(textWindowTransparency) << endl;
		myfile.close();
	}
	else
		cout << "Failed to write Config.txt";
}

void Config::parse(string configFile)
{
	Utility util;

	ifstream myFileStream(configFile);
	if (!myFileStream.is_open()) {
		cout << "Failed to open file" << endl;
	}

	string line;			// An ENTIRE row of the file stored as a string
	string tempStr;			// Temporary string used as we parse line
	int tempInt;			// Temporary int used as we parse line

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	masterVolume = stof(tempStr);
	
	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	bgmVolume = stof(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	voiceVolume = stof(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	sfxVolume = stof(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	fullScreen = stoi(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	manualTextSpeed = stof(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	autoTextSpeed = stof(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	autoTextWaitTime = stof(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	textFade = util.str2bool(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	skipUnreadText = util.str2bool(tempStr);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, fontFileName);

	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	textWindowTransparency = stof(tempStr);
}

string Config::getTitle()
{
	return windowTitle;
}

int Config::getWindowWidth()
{
	return windowWidth;
}

int Config::getWindowHeight()
{
	return windowHeight;
}

float Config::getFps()
{
	return fps;
}
