#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"

class PlayCommand : public ScriptCommand
{
public:

	PlayCommand(std::vector<std::string> args);
	~PlayCommand();

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	std::string objectName;			// the name of the character or background
	std::string objectSubname;		// the expression of character or time-of-the-day of background

	float time;
	float finalVolume;
	float currentVolume = 0;

	int flagType;
	int objectType;
	bool repeat = false;
	bool clearPrev = true;
	bool finishedAction = false;
	bool played = false;

	sf::Music* music = nullptr;

	const static int FLAG_FADEIN = 1;
	const static int FLAG_NONE = 0;

	const static int OBJECT_BGM = 0;
	const static int OBJECT_VOICE = 1;
	const static int OBJECT_SFX = 2;
};