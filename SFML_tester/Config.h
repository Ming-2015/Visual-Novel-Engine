#pragma once
#include <string>

using namespace std;

class Config 
{
public:

	Config();	// empty constructor
	void parse(string configFile);	// parse the config file to get the saved options from previous playthrough
	void save(string configFile);	// save the config options into the file so that the opts are remembered

	string getTitle();				// title of the window
	int getWindowWidth();			// width of the window
	int getWindowHeight();			// height of the window
	float getFps();					// FPS of the window

	float masterVolume;				// Master volume
	float bgmVolume;				// BGM volume
	float voiceVolume;				// Voice volume
	float sfxVolume;				// SFX volume
	int fullScreen;					// fullscreen option
	float manualTextSpeed;			// text speed in manual mode
	float autoTextSpeed;			// text speed in auto mode
	float autoTextWaitTime;			// wait time after auto text finished displaying
	bool textFade;					// toggle text fade animation
	bool skipUnreadText;			// allow skipping read/unread text (not sure how to implement this yet)
	string fontFileName;			// name of the font file (.ttf) we are using for the text
	float textWindowTransparency;	// text window transparency (alpha)

private:
	const static int WINDOWED = 1, FULLSCREEN = 2, FULLSCREEN_WINDOW = 3;

	int windowWidth, windowHeight;	// window widht and height
	string windowTitle;				// window title
	float fps;						// fps 

};