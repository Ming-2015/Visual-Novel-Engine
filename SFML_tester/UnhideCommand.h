#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"
#include "Config.h"

class UnhideCommand : public ScriptCommand
{
public:

	UnhideCommand(std::vector<std::string> args);
	~UnhideCommand();

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	// the dialogue/monologue being displayed
	std::vector<std::string> displayLines;

	// type of animation
	int animationType;

	// type of object: choice or line
	int objectType;

	const static int ANIMATION_FADEIN = 1;
	const static int ANIMATION_NONE = 0;

	const static int OBJECT_TEXTBOX = 0;

	float currentAlpha = 0;
	float finalAlpha = 255;
	bool doneAnimation = false;

	float time = 0;
};