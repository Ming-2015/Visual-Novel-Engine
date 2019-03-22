#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"

class StartCommand : public ScriptCommand
{
public:

	StartCommand(std::vector<std::string> args);
	~StartCommand();

	// serialize the file
	StartCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	bool initialized = false;
	float currentHeight = 0;

	float maxHeight;
	float time;

	int objectType;
	int animationType;

	const static int ANIMATION_WAIT = 1;
	const static int ANIMATION_NONE = 0;

	const static int OBJECT_CINEMATIC = 0;
};