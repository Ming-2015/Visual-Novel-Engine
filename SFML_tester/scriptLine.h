#pragma once
#ifndef scriptLine_h
#define scriptLine_h
#include <iostream>
#include "charPic.h"
using namespace std;

class scriptLine {
public:
	int currentLineID;					// ID of current line
	int nextLineID;						// ID of next line (may not be needed when isChoice = false;
	bool isChoice;						// Is user input prompted after this line?
	string line;						// Actual Script Line
	string choices[4];					// If isChoice, will hold the line ID of the possible next lines  CHANGE SIZE
	int userChoice;						// If isChoice, this will hold user input and will serve as index for choices[]
	charPic picture[3];					// Pictures included in the "frame"  CHANGE SIZE
	string nextFileNames[];
};




#endif