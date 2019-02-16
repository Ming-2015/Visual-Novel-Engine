#pragma once
#include "scriptLine.h"
#include "Utility.h"
#include "ItemImage.h"
#include "logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class ScriptLine 
{
public:

	string dialogue;						// Actual Script Line
	string name;							// Character name to be displayed
	TextboxImage* textboxImage = nullptr;	// Textbox 
	bool hideTextbox = true;				// hide textbox

	string voiceFileName;				// voice file name, "" if none

	string BGMFileName;					// background music file name

	bool isChoice;						// Is user input prompted after this line?
	int numChoices;						// the number of choices to be displayed to the User
	vector<string> choices;				// If isChoice, will hold the line ID of the possible next lines  CHANGE SIZE
	vector<string> nextFileNames;		// script file names of the next line (if isChoice is true)

	vector<CharacterImage *> characterImages;	
	vector<BackgroundImage *> backgroundImages;

public:
	ScriptLine();
	~ScriptLine();

	void setCharacter(const string& name, const string& expression, 
		float xPos, float yPos, 
		float fadeTime = 0.5f,
		float xScale = 1.0f, float yScale = 1.0f,
		bool clockwise = false, float angle = 0
	);

	void setBackground(const string& name, const string& timeOfTheDay,
		float xPos, float yPos,
		float fadeTime = 0.5f,
		float xScale = 1.0f, float yScale = 1.0f,
		bool clockwise = false, float angle = 0
	);

	void setCharacterAlpha(const string& name, float alpha);
	void setBackgroundAlpha(const string& name, float alpha);
	void setCharacterRotation(const string& name, const string& expression, bool clockwise, float degree);
	void setBackgroundRotation(const string& name, const string& expression, bool clockwise, float degree);
	void setBackgroundZoom(const string& name, const string& expression, float xScale, float yScale);
	void setCharacterZoom(const string& name, const string& expression, float xScale, float yScale);

	void removeCharacter(const string& name);
	void removeBackground(const string& name);
	void removeAllCharacters();
	void removeAllBackgrounds();

	void setDialogue(const string& displayname, const string& str);
	void changeCharacterPosition(const string& name, float xPos, float yPos);

private:
	std::string addAllNewLines(std::string str, unsigned int lineLength);
	std::string addNewLineToPrevWord(std::string str, unsigned int pos);
};