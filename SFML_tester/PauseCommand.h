#pragma once

#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"

class PauseCommand : public ScriptCommand
{
public:

	PauseCommand(std::vector<std::string> args);
	~PauseCommand();

	// serialize the file
	PauseCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

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