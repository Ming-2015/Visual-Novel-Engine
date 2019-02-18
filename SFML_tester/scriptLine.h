#pragma once
#include "scriptLine.h"
#include "Utility.h"
#include "ItemImage.h"
#include "logger.h"
#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
using namespace std;

class ScriptLine 
{
public:

	string dialogue;						// Actual Script Line
	string name;							// Character name to be displayed
	TextboxImage* textboxImage = nullptr;	// Textbox 
	bool hideTextbox = true;				// hide textbox

	vector<sf::Music*> bgm;		// note that all music will be directly managed by commands
	vector<sf::Music*> voices;			
	vector<sf::Music*> sfx;

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
		float fadeTime = 0.0f
	);

	void setBackground(const string& name, const string& timeOfTheDay,
		float xPos, float yPos,
		float fadeTime = 0.0f
	);

	void setCharacterAlpha(const string& name, float alpha);
	void setBackgroundAlpha(const string& name, float alpha);
	void setCharacterRotation(const string& name, const string& expression, bool clockwise, float degree);
	void setBackgroundRotation(const string& name, const string& expression, bool clockwise, float degree);
	void setCharacterRotationRel(const string& name, const string& expression, bool clockwise, float degree);
	void setBackgroundRotationRel(const string& name, const string& expression, bool clockwise, float degree);
	void moveBackgroundRel(const string& name, const string& expression, float x1, float y1);
	void moveCharacterRel(const string& name, const string& expression, float x1, float y1);
	void setBackgroundZoom(const string& name, const string& expression, float xScale, float yScale);
	void setCharacterZoom(const string& name, const string& expression, float xScale, float yScale);
	float getBackgroundBeginScaleX(const string& name, const string& expression);
	float getBackgroundBeginScaleY(const string& name, const string& expression);
	float getCharacterBeginScaleX(const string& name, const string& expression);
	float getCharacterBeginScaleY(const string& name, const string& expression);

	void removeCharacter(const string& name);
	void removeBackground(const string& name);
	void removeAllCharacters();
	void removeAllBackgrounds();

	void setDialogue(const string& displayname, const string& str);
	void changeCharacterPosition(const string& name, float xPos, float yPos);

	sf::Music* setBgm(const string& groupname, const string& filename, bool clearOthers = true, bool repeat = true, float volume = 1.0f);
	sf::Music* setVoice(const string& groupname, const string& filename, bool clearOthers = true, bool repeat = false, float volume = 1.0f);
	sf::Music* setSfx(const string& groupname, const string& filename, bool clearOthers = false, bool repeat = false, float volume = 1.0f);

	void setBgmVolume(float volume);
	void setVoiceVolume(float volume);
	void setSfxVolume(float volume);

	void setBgmVolume(sf::Music* m, float volume);
	void setVoiceVolume(sf::Music* m, float volume);
	void setSfxVolume(sf::Music* mu, float volume);

private:
	std::string addAllNewLines(std::string str, unsigned int lineLength);
	std::string addNewLineToPrevWord(std::string str, unsigned int pos);
};