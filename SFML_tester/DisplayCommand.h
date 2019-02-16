#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"
#include "Config.h"

class DisplayCommand : public ScriptCommand
{
public:

	DisplayCommand(std::vector<std::string> args);
	~DisplayCommand();

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	// the name of the character talking
	std::string displayName;	

	// the dialogue/monologue being displayed
	std::vector<std::string> displayLines;

	// if the display command is a choice, what are the next locations?
	std::vector<std::string> sceneLabels;

	// type of animation
	int animationType;

	// type of object: choice or line
	int objectType;

	const static int ANIMATION_GOREY = 2;
	const static int ANIMATION_EXCLAIMATION = 1;
	const static int ANIMATION_NONE = 0;
	const static int ANIMATION_INSTANT = 3;

	const static int OBJECT_LINE = 0;
	const static int OBJECT_CHOICE = 1;

	// to help animate the display line
	std::string currentLine;
	float timer;
	int currentLineIndex, currentCharIndex;

	std::string assembleString(const std::vector<std::string>& lines, int lineIndex, int charIndex);
};