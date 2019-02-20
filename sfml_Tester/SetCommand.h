#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"

class SetCommand : public ScriptCommand
{
public:

	SetCommand(std::vector<std::string> args);
	~SetCommand();

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	std::string objectName;			// the name of the character or background
	std::string objectSubname;		// the expression of character or time-of-the-day of background

	float x1, y1;
	float time;

	int animationType;
	float alpha;

	int objectType;

	const static int ANIMATION_FADEIN = 1;
	const static int ANIMATION_NONE = 0;

	const static int OBJECT_CHARACTER = 0;
	const static int OBJECT_BACKGROUND = 1;

	bool initiailized = true;
};
