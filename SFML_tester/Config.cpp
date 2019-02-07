#include "Config.h"

Config::Config()
{
	windowWidth = 1600;
	windowHeight = 900;
	windowTitle = "Visual Novel Engine";
	fps = 60;
}

void Config::parse(string configFile)
{

}

string Config::getTitle()
{
	return windowTitle;
}

int Config::getWindowWidth()
{
	return windowWidth;
}

int Config::getWindowHeight()
{
	return windowHeight;
}

float Config::getFps()
{
	return fps;
}
