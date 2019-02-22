#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "ScriptCommand.h"

using namespace std;

class RotateCommand : public ScriptCommand
{
public:

	RotateCommand(vector<string> args);
	~RotateCommand();

	// serialize the file
	RotateCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	string objectTypeName;		//Background | Character
	string flag;				// Wait | none

	string objectName;			// name of character | Background
	string objectSubname;		// expression of character or time-of-the-day for background
	string whichWay;

	float time;

	int objectType;
	int animationType;

	float finalDegree;
	float currentRotate;
	float angleDiff;
	float tempAngle;

	bool clockwise;
	bool relative;
	bool stopRotate = false;

	const static int OBJECT_CHARACTER = 0;
	const static int OBJECT_BACKGROUND = 1;

	const static int ANIMATION_ROTATE = 1;
	const static int ANIMATION_NONE = 0;

};