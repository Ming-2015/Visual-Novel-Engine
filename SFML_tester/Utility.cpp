#include "Utility.h"
#include "charPic.h"
#include "scriptLine.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

Utility::Utility()
{

}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

bool Utility::str2bool(string s) {
	if (s == "true")
		return true;
	else if (s == "false")
		return false;
	throw exception("bad string");
}

void Utility::getLine(void) {

	int currentLineID;
	string s_line;
	string name;
	bool isChoice;
	int numChoices;
	vector<string> choices;
	vector<string> nextFileNames;
	vector<int> nextLineIDs;
	int userChoice;
	int numChars;
	vector<CharPic> charpics;
	string s;

	ifstream myFileStream("test.txt");
	if (!myFileStream.is_open()) {
		cout << "Failed to open file" << endl;
	}

	string tempStr;
	int tempInt;
	string line;
	// ON HOLD, CREATE UTILITY CLASS

	stringstream ss(line);
	getline(ss, tempStr, '&');
	currentLineID = stoi(tempStr);
	getline(ss, s_line, '&');
	getline(ss, name, '&');
	getline(ss, tempStr, '&');
	isChoice = str2bool(tempStr);
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

	myFileStream.close();

	if (!myFileStream.eof())
	{
		cout << "Error!\n";
	}
}