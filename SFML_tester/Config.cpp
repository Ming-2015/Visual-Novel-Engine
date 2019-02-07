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


	// I have yet to test if this is the right way to keep reading new line.
	// The rename ss convention is TERRIBLE, will test tomorrow.
	getline(myFileStream, line);
	stringstream ss(line);
	getline(ss, tempStr, '\t');
	getline(ss, tempStr);
	masterVolume = stof(tempStr);
	
	getline(myFileStream, line);
	stringstream tt(line);
	getline(tt, tempStr, '\t');
	getline(tt, tempStr);
	bgmVolume = stof(tempStr);

	getline(myFileStream, line);
	stringstream yy(line);
	getline(yy, tempStr, '\t');
	getline(yy, tempStr);
	voiceVolume = stof(tempStr);

	getline(myFileStream, line);
	stringstream zz(line);
	getline(zz, tempStr, '\t');
	getline(zz, tempStr);
	sfxVolume = stof(tempStr);

	getline(myFileStream, line);
	stringstream qq(line);
	getline(qq, tempStr, '\t');
	getline(qq, tempStr);
	fullScreen = stoi(tempStr);

	getline(myFileStream, line);
	stringstream xx(line);
	getline(xx, tempStr, '\t');
	getline(xx, tempStr);
	manualTextSpeed = stof(tempStr);

	getline(myFileStream, line);
	stringstream cc(line);
	getline(cc, tempStr, '\t');
	getline(cc, tempStr);
	autoTextSpeed = stof(tempStr);

	getline(myFileStream, line);
	stringstream vv(line);
	getline(vv, tempStr, '\t');
	getline(vv, tempStr);
	autoTextWaitTime = stof(tempStr);

	getline(myFileStream, line);
	stringstream bb(line);
	getline(bb, tempStr, '\t');
	getline(bb, tempStr);
	textFade = util.str2bool(tempStr);

	getline(myFileStream, line);
	stringstream nn(line);
	getline(nn, tempStr, '\t');
	getline(nn, tempStr);
	skipUnreadText = util.str2bool(tempStr);

	getline(myFileStream, line);
	stringstream mm(line);
	getline(mm, tempStr, '\t');
	getline(mm, fontFileName);

	getline(myFileStream, line);
	stringstream ll(line);
	getline(ll, tempStr, '\t');
	getline(ll, tempStr);
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
