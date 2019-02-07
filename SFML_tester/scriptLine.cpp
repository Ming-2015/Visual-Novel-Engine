#include "scriptLine.h"
using namespace std;

ScriptLine::ScriptLine() 
{

}

void ScriptLine::parse(ifstream& myFileStream)
{
	choices = vector<string>();
	nextFileNames = vector<string>();
	nextLineIDs = vector<int>();

	charPics = vector<CharPic>();
	if (!myFileStream.is_open()) {
		string err = "Invalid script file";
		LOGGER->Log("ScriptLine", err);
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
	isChoice = UTILITY->str2bool(tempStr);
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
		LOGGER->Log("ScriptLine", "Unable to close file");
	}
}