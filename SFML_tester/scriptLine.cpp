#include "scriptLine.h"
#include "Utility.h"
#include "charPic.h"
#include "scriptLine.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

ScriptLine::ScriptLine() 
{
	choices = vector<string>();
	nextFileNames = vector<string>();
	nextLineIDs = vector<int>();

	charPics = vector<CharPic>();

	Utility util;

	ifstream myFileStream("test.txt");
	if (!myFileStream.is_open()) {
		cout << "Failed to open file" << endl;
	}

	string line;			// An ENTIRE row of the file stored as a string
	string tempStr;			// Temporary string used as we parse line
	int tempInt;			// Temporary int used as we parse line

	stringstream ss(line);
	getline(ss, tempStr, '&');
	currentLineID = stoi(tempStr);
	getline(ss, s_line, '&');
	getline(ss, name, '&');

	getline(ss, tempStr, '&');
	isChoice = util.str2bool(tempStr);
	getline(ss, tempStr, '&');
	numChoices = stoi(tempStr);
	for (int i = 0; i < numChoices; i++) {
		getline(ss, tempStr, '&');
		choices.push_back(tempStr);
	}
	for (int i = 0; i < numChoices; i++) {
		getline(ss, tempStr, '&');
		nextFileNames.push_back(tempStr);
	}
	for (int i = 0; i < numChoices; i++) {
		getline(ss, tempStr, '&');
		tempInt = stoi(tempStr);
		nextLineIDs.push_back(tempInt);
	}
	userChoice = 1;

	getline(ss, tempStr, '&');
	numChars = stoi(tempStr);
	for (int i = 0; i < numChars; i++) {
		getline(ss, tempStr, '&');
		charPics[i].picName = tempStr;
		getline(ss, tempStr, '&');
		charPics[i].xPos = stoi(tempStr);
		getline(ss, tempStr, '&');
		charPics[i].yPos = stoi(tempStr);
	}

	myFileStream.close();

	if (!myFileStream.eof())
	{
		cout << "Error!\n";
	}
}