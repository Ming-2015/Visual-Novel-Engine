#pragma once
#include "Utility.h"
#include "Logger.h"
#include "Global.h"
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#define CONFIG Config::GetConfig()

enum FullscreenOpts
{
	fullscreen = 1,
	windowed = 2,
	fullscreenWindowed = 3,
};

class Config 
{
public:

	void parse(string configFile);	// parse the config file to get the saved options from previous playthrough
	void write(string configFile);  // writes a new config file when user changes config settings (saves over old one)

	string getTitle();				// title of the window
	int getWindowWidth();			// width of the window
	int getWindowHeight();			// height of the window
	float getFps();					// FPS of the window

	float masterVolume;				// Master volume
	float bgmVolume;				// BGM volume
	float voiceVolume;				// Voice volume
	float sfxVolume;				// SFX volume
	FullscreenOpts enableFullscreen;// fullscreen option
	float manualTextSpeed;			// text speed in manual mode
	float autoTextSpeed;			// text speed in auto mode
	float autoTextWaitTime;			// wait time after auto text finished displaying
	bool textFade;					// toggle text fade animation
	bool skipUnreadText;			// allow skipping read/unread text (not sure how to implement this yet)
	string displayTextFontName;		// name of the font file (.ttf) we are using for the text
	float textWindowAlpha;			// text window transparency (alpha)
	int windowWidth, windowHeight;	// window width and height

	const int defaultWidth = 1600, defaultHeight = 900;

	static const string CF_MASTER_VOLUME;
	static const string CF_BGM_VOLUME;
	static const string CF_VOICE_VOLUME;
	static const string CF_SFX_VOLUME;
	static const string CF_FULLSCREEN;
	static const string CF_MANUAL_TEXT_SPEED;
	static const string CF_AUTO_TEXT_SPEED;
	static const string CF_AUTO_TEXT_WAIT_TIME;
	static const string CF_TEXT_FADE;
	static const string CF_SKIP_UNREAD_TEXT;
	static const string CF_DISPLAY_TEXT_FONT_NAME;
	static const string CF_TEXT_WINDOW_ALPHA;
	static const string CF_WINDOW_WIDTH;
	static const string CF_WINDOW_HEIGHT;

	static Config * GetConfig();
	static void Cleanup();

private:
	string windowTitle;				// window title
	float fps;						// fps 

	void init();	
	Config();	// empty constructor

	static Config * currentConfig;
};