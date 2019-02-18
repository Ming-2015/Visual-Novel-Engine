#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include "ScriptCommand.h"
using namespace std;

class FlashCommand : public ScriptCommand
{
public:
	FlashCommand(vector<string> args);
	~FlashCommand();

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;
	string objectName;

	int numFlash;
	int flashCount;
	int showBlack;
	float time;
	bool keep;
	sf::Clock clock;

	int animationType;
	float alpha;

	int objectType;

	const static int ANIMATION_FLASHFADEOUT = 1;
	const static int ANIMATION_FLASH = 0;

	const static int OBJECT_CHARACTER = 0;
	const static int OBJECT_BACKGROUND = 1;
	const static int OBJECT_ALL = 2;

	bool doneFade = false;
	bool doneFlash = false;
};
