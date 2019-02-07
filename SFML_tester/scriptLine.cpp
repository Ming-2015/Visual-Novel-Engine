#include "ScriptLine.h"

ScriptLine::ScriptLine() {
	choices = vector<string>();
	nextFileNames = vector<string>();
	nextLineIDs = vector<int>();

	charPics = vector<CharPic>();
}