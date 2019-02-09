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

	if (myFileStream.eof()) return;

	getline(myFileStream, line);	// read a line from the file

	if (line.find('|') == std::string::npos) return;

	stringstream ss(line);
	getline(ss, tempStr, '|');
	currentLineID = stoi(tempStr);
	getline(ss, currentFileName, '|');
	getline(ss, s_line, '|');
	getline(ss, name, '|');

	getline(ss, tempStr, '|');
	isChoice = UTILITY->str2bool(tempStr);
	getline(ss, tempStr, '|');
	numChoices = stoi(tempStr);
	if (numChoices == 0) {
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
	}
	else if (numChoices == 2) {
		getline(ss, tempStr, '|');
		choices.push_back(tempStr);
		getline(ss, tempStr, '|');
		choices.push_back(tempStr);
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
	}
	else
	{
		for (int i = 0; i < numChoices; i++) {
			getline(ss, tempStr, '|');
			choices.push_back(tempStr);
			getline(ss, tempStr, '|');
			choices.push_back(tempStr);
			getline(ss, tempStr, '|');
			choices.push_back(tempStr);
			getline(ss, tempStr, '|');
			choices.push_back(tempStr);
		}
	}
	if (numChoices == 0) {
		getline(ss, tempStr, '|');
		nextFileNames.push_back(tempStr);
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
	}
	else if (numChoices == 2) {
		getline(ss, tempStr, '|');
		nextFileNames.push_back(tempStr);
		getline(ss, tempStr, '|');
		nextFileNames.push_back(tempStr);
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
	}
	else
	{
		for (int i = 0; i < numChoices; i++) {
			getline(ss, tempStr, '|');
			nextFileNames.push_back(tempStr);
			getline(ss, tempStr, '|');
			nextFileNames.push_back(tempStr);
			getline(ss, tempStr, '|');
			nextFileNames.push_back(tempStr);
			getline(ss, tempStr, '|');
			nextFileNames.push_back(tempStr);
		}
	}
	if (numChoices == 0) {
		getline(ss, tempStr, '|');
		nextLineIDs.push_back(stoi(tempStr));
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
	}

	else if (numChoices == 2) {
		getline(ss, tempStr, '|');
		nextLineIDs.push_back(stoi(tempStr));
		getline(ss, tempStr, '|');
		nextLineIDs.push_back(stoi(tempStr));
		getline(ss, tempStr, '|');
		getline(ss, tempStr, '|');
	}
	else
	{
		for (int i = 0; i < numChoices; i++) {
			getline(ss, tempStr, '|');
			nextLineIDs.push_back(stoi(tempStr));
			getline(ss, tempStr, '|');
			nextLineIDs.push_back(stoi(tempStr));
			getline(ss, tempStr, '|');
			nextLineIDs.push_back(stoi(tempStr));
			getline(ss, tempStr, '|');
			nextLineIDs.push_back(stoi(tempStr));
		}
	}

	userChoice = 1;

	getline(ss, tempStr, '|');
	numChars = stoi(tempStr);
	for (int i = 0; i < numChars; i++) {
		charPics.push_back(CharPic());
		getline(ss, tempStr, '|');
		charPics[i].picName = tempStr;
		getline(ss, tempStr, '|');
		charPics[i].xPos = stoi(tempStr);
		getline(ss, tempStr, '|');
		charPics[i].yPos = stoi(tempStr);

		/*
		CharPic* temp = new CharPic();
		getline(ss, tempStr, '|');
		temp->picName = tempStr;
		getline(ss, tempStr, '|');
		temp->xPos = stoi(tempStr);
		getline(ss, tempStr, '|');
		temp->yPos = stoi(tempStr);
		charPics.push_back(*temp);
		delete temp;
		*/
	}
}