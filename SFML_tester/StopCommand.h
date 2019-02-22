#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"

class StopCommand : public ScriptCommand
{
public:

	StopCommand(std::vector<std::string> args);
	~StopCommand();

	// serialize the file
	StopCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	std::string objectName;			// the name of the character or background
	std::string objectSubname;		// the expression of character or time-of-the-day of background

	float time;
	float finalVolume = 0;
	float currentVolume = 1;
	float prevVolume = 1;

	int flagType;
	int objectType;
	bool finishedAction = false;

	const static int FLAG_FADEOUT = 1;
	const static int FLAG_NONE = 0;

	const static int OBJECT_BGM = 0;
	const static int OBJECT_VOICE = 1;
	const static int OBJECT_SFX = 2;
	const static int OBJECT_ALL = 3;
};