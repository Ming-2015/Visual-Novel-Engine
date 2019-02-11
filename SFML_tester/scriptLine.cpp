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

	// ID
	getline(ss, tempStr, '|');
	if (tempStr != "")
		currentLineID = stoi(tempStr);

	// main message line
	getline(ss, tempStr, '|');
	if (tempStr != "")
		s_line = tempStr;

	// display name - name has to be properly inputted every time
	getline(ss, name, '|');

	// background image file
	getline(ss, tempStr, '|');
	if (tempStr != "")
		backgroundFileName = tempStr;

	// background file name
	getline(ss, tempStr, '|');
	if (tempStr != "")
		backgroundChange = UTILITY->str2bool(tempStr);

	// textbox file name
	getline(ss, tempStr, '|');
	if (tempStr != "")
		textboxFileName = tempStr;

	// change textbox?
	getline(ss, tempStr, '|');
	if (tempStr != "")
		textboxChange = UTILITY->str2bool(tempStr);

	// is currently displaying choices?
	getline(ss, tempStr, '|');
	if (tempStr != "")
		isChoice = UTILITY->str2bool(tempStr);

	// number of choices involved
	getline(ss, tempStr, '|');
	if (tempStr != "")
		numChoices = stoi(tempStr);

	// pushing the choice strings
	choices.clear();
	if (numChoices < 4) {
		for (int i = 0; i < 4; i++)
		{
			getline(ss, tempStr, '|');
			if (i < numChoices)
			{
				choices.push_back(tempStr);
			}
		}
	}
	else
	{
		for (int i = 0; i < numChoices; i++) {
			getline(ss, tempStr, '|');
			choices.push_back(tempStr);
		}
	}

	// pushing the next file names
	nextFileNames.clear();
	if (numChoices < 4) {
		for (int i = 0; i < 4; i++)
		{
			getline(ss, tempStr, '|');
			if (i < numChoices)
			{
				nextFileNames.push_back(tempStr);
			}
		}
	}
	else
	{
		for (int i = 0; i < numChoices; i++) {
			getline(ss, tempStr, '|');
			nextFileNames.push_back(tempStr);
		}
	}

	// pushing the next line IDs
	nextLineIDs.clear();
	if (numChoices < 4) {
		for (int i = 0; i < 4; i++)
		{
			getline(ss, tempStr, '|');
			if (i < numChoices)
			{
				nextLineIDs.push_back(stoi(tempStr));
			}
		}
	}
	else
	{
		for (int i = 0; i < numChoices; i++) {
			getline(ss, tempStr, '|');
			nextLineIDs.push_back(stoi(tempStr));
		}
	}

	// getting number of characters to display
	getline(ss, tempStr, '|');
	if (tempStr != "")
		numChars = stoi(tempStr);

	// pushing the character pic info into a temporary vector
	std::vector<CharPic> tempCharPic(numChars, CharPic());
	for (int i = 0; i < numChars; i++) {
		getline(ss, tempStr, '|');
		if (tempStr != "")
			tempCharPic[i].picName = tempStr;
		else if (charPics.size() > i)
			tempCharPic[i].picName = charPics[i].picName;

		getline(ss, tempStr, '|');
		if (tempStr != "")
			tempCharPic[i].xPos = stoi(tempStr);
		else if (charPics.size() > i)
			tempCharPic[i].xPos = charPics[i].xPos;

		getline(ss, tempStr, '|');
		if (tempStr != "")
			tempCharPic[i].yPos = stoi(tempStr);
		else if (charPics.size() > i)
			tempCharPic[i].yPos = charPics[i].yPos;
	}

	// replace the old charpics with the temp
	charPics.clear();
	charPics = tempCharPic;
}