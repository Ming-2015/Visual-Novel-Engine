#pragma once
#include <string>
#include "Logger.h"

using namespace std;

enum FullscreenOpts
{
	fullscreen = 1,
	windowed = 2,
	fullscreenWindowed = 3,
};

class Config 
{
public:

	Config();	// empty constructor
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
	string fontFileName;			// name of the font file (.ttf) we are using for the text
	float textWindowAlpha;	// text window transparency (alpha)

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
	static const string CF_FONT_FILE_NAME;
	static const string CF_TEXT_WINDOW_ALPHA;

private:
	int windowWidth, windowHeight;	// window widht and height
	string windowTitle;				// window title
	float fps;						// fps 

	void init();
};