#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "ScriptCommand.h"

using namespace std;

class MoveCommand : public ScriptCommand
{
public:

	MoveCommand(vector<string> args);
	~MoveCommand();

	// serialize the file
	MoveCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	string objectTypeName;		//Background | Character
	string flag;				// Wait | none

	string objectName;			// name of character | Background
	string objectSubname;		// expression of character or time-of-the-day for background

	int objectType;
	int animationType;

	float startX, startY;
	float endX, endY;

	bool doneX = false, doneY = false;
	
	float time;

	float xDiff, yDiff;
	float xOffset, yOffset;
	bool relative;
	bool stopMove = false;

	const static int OBJECT_CHARACTER = 0;
	const static int OBJECT_BACKGROUND = 1;

	const static int ANIMATION_MOVE = 1;
	const static int ANIMATION_NONE = 0;
};