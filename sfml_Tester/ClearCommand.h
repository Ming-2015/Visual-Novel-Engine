#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include "ScriptCommand.h"
using namespace std;

class ClearCommand : public ScriptCommand
{
public:
	ClearCommand(vector<string> args);
	~ClearCommand();

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	float time;

	int animationType;
	float alpha;

	int objectType;

	const static int ANIMATION_FADEOUT = 1;
	const static int ANIMATION_NONE = 0;

	const static int OBJECT_CHARACTER = 0;
	const static int OBJECT_BACKGROUND = 1;
};