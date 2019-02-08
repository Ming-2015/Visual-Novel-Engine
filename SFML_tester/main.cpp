#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Logger.h"
#include "Engine.h"

using namespace std;

int main()
{
	Engine engine;
	int ret = engine.start();

	string msg = "Returned with code " + to_string(ret);
	LOGGER->Log("main", msg);

	return ret;
}