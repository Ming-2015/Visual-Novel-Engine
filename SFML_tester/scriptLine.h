#pragma once
#include "scriptLine.h"
#include "Utility.h"
#include "charPic.h"
#include "logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class ScriptLine 
{
public:
	int currentLineID;					// ID of current line
	string currentFileName;				// Name of current script file
	string s_line;						// Actual Script Line
	string name;						// Character name to be displayed

	string backgroundFileName;			// Background image file name
	string voiceFileName;				// voice file name, "" if none
	string BGMFileName;					// background music file name

	bool isChoice;						// Is user input prompted after this line?
	int numChoices;						// the number of choices to be displayed to the User
	vector<string> choices;				// If isChoice, will hold the line ID of the possible next lines  CHANGE SIZE
	vector<string> nextFileNames;		// script file names of the next line (if isChoice is true)
	vector<int> nextLineIDs;			// ID of next line (if isChoice is true)
	int userChoice;						// If isChoice, this will hold user input and will serve as index for choices[]

	int numChars;						// the number of characters to be shown on the screen
	vector<CharPic> charPics;			// Pictures included in the "frame"  CHANGE SIZE

	ScriptLine();
	void parse(ifstream& file);
};