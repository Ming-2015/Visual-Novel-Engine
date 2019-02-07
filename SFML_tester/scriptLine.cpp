
#include "scriptLine.h"
#include "Utility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

ScriptLine::ScriptLine() 
{
	choices = vector<string>();
	nextFileNames = vector<string>();
	nextLineIDs = vector<int>();

	charPics = vector<CharPic>();

	Utility function;
	function.getLine();
}