#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>

#include "scriptLine.h"
#include "Utility.h"
#include "ItemImage.h"
#include "logger.h"
#include "Config.h"
#include "CharacterImage.h"
#include "BackgroundImage.h"
#include "TextboxImage.h"
#include "ChoiceImage.h"

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

	bool isChoice = false;					// Is user input prompted after this line?
	vector<ChoiceImage *> choiceImages;		// If isChoice, will hold the line ID of the possible next lines  CHANGE SIZE

	vector<CharacterImage *> characterImages;	
	vector<BackgroundImage *> backgroundImages;

	std::string filename;
	ifstream file;

public:
	ScriptLine();
	~ScriptLine();

	void setChoices(const vector<string>& choices, const vector<string>& flags);
	void clearChoices();

	void setCharacter(const string& name, const string& expression, 
		float xPos, float yPos, 
		float fadeTime = 0.5f
	);

	void setBackground(const string& name, const string& timeOfTheDay,
		float xPos, float yPos,
		float fadeTime = 0.0f
	);

	void setCharacterAlpha(const string& name, float alpha);
	void setBackgroundAlpha(const string& name, float alpha);

	void setAllCharacterAlpha(float alpha);
	void setAllBackgroundAlpha(float alpha);

	void setTextboxAlpha(float alpha);
	
	void setCharacterRotation(const string& name, const string& expression, bool clockwise, float degree);
	void setBackgroundRotation(const string& name, const string& expression, bool clockwise, float degree);
	
	void setCharacterRotationRel(const string& name, const string& expression, bool clockwise, float degree);
	void setBackgroundRotationRel(const string& name, const string& expression, bool clockwise, float degree);

	void setCharacterShader(const string& name, const string&shaderSrc);
	void setBackgroundShader(const string& name, const string&shaderSrc);
	void tickCharacterShader(const string& name, bool isTrue);
	void tickBackgroundShader(const string& name, bool isTrue);
	void setCharacterBlurRadius(const string& name, float param);
	void setBackgroundBlurRadius(const string& name, float param);
	float getCharacterBeginBlurR(const string& name);
	float getBackgroundBeginBlurR(const string& name);

	void moveBackgroundRel(const string& name, const string& expression, float x1, float y1);
	void moveCharacterRel(const string& name, const string& expression, float x1, float y1);
	
	void setBackgroundZoom(const string& name, const string& expression, float xScale, float yScale, float xPos = 0, float yPos = 0);
	void setCharacterZoom(const string& name, const string& expression, float xScale, float yScale, float xPos = 0, float yPos = 0);
	
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

	void setBgmVolume(float volume, bool relative = false);
	void setVoiceVolume(float volume, bool relative = false);
	void setSfxVolume(float volume, bool relative = false);

	void setBgmVolume(sf::Music* m, float volume);
	void setVoiceVolume(sf::Music* m, float volume);
	void setSfxVolume(sf::Music* mu, float volume);

	void stopBgm();
	void stopSfx();
	void stopVoice();

	void pauseBgm();
	void pauseSfx();
	void pauseVoice();

	void resumeBgm();
	void resumeSfx();
	void resumeVoice();

	void readNewFile(std::string filename);
	void readNewFileToAnchor(std::string filename, std::string anchor);

private:

};