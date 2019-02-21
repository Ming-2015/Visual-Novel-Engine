#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"

using namespace std;

class BlurCommand : public ScriptCommand
{
public:

	BlurCommand(vector<string> args);
	~BlurCommand();

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	string objectTypeName;
	string flag;

	string objectName;

	float time;
	bool initialized = false;

	int animationType;
	float blurRadius;
	float currentBlurR;

	int objectType;

	const static int ANIMATION_GRADUAL = 1;
	const static int ANIMATION_NONE = 0;

	const static int OBJECT_CHARACTER = 0;
	const static int OBJECT_BACKGROUND = 1;
	const static int OBJECT_ALL = 2;

	bool relative;
	bool firstLoopRel = true;
	bool lastLoopRel = false;
	float blurDiff;
	string srcFile = "resources/blur.frag";
};