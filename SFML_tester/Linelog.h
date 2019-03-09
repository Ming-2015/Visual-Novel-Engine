#pragma once
#include <vector>
#include "CharacterImage.h"
#include "BackgroundImage.h"
#include "LineLogItem.h"
#include "LogBar.h"

class LineLog {

public:

	LineLog();
	~LineLog();

	void addLogItem(
		std::string name,
		std::string line,
		std::string musicFile,
		std::string voiceFile,
		std::string scriptFile,
		unsigned int scriptFileSeekg,
		std::set< std::string > flags
	);

	void addLogItem(LineLogItem logItem);

	std::string getName(int numPrev);
	std::string getLine(int numPrev);
	std::string getMusicFile(int numPrev);
	std::string getVoiceFile(int numPrev);
	std::string getScriptFile(int numPrev);
	unsigned int getScriptFilePos(int numPrev);
	std::set< std::string > getFlags(int numPrev);

	bool shouldJump(bool reset = false);
	int getJumpIdx() const;
	int getBottomIdx() const;			// the bottom index of the log being displayed
	int getCurrentHoverIndex() const;	// this is not relative to bottom (absolute)
	int getLogCount() const;
	bool shouldDisplay() const;

	void displayLog(bool disp);

	void handleInput(sf::Event& e, sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
	void update(float delta_t);

	const int MAX_LOGITEMS = 50;

private:

	// Items to keep track of each time a new line is added
	std::vector<LineLogItem> logItems;

	// should display the log to the player?
	bool displaying = false;

	// which position is the player on in the log?
	int bottom = 0;

	// if less than 0, nothing is being hovered. This number is relative to bottom
	int currentlyHovered = -1;	

	// should we start jumping?
	bool jumping = false;
	int jumpIdx;

	// how many items are there in the log?
	//int logCount = 0;

	sf::Texture logBackgroundImage;
	sf::Sprite logBackground;

	std::vector<LogBar*> logBars;
};