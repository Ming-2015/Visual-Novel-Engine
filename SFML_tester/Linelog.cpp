#include "Linelog.h"

LineLog::LineLog()
{
	if (!logBackgroundImage.loadFromFile(GLOBAL->AssetRoot + "logBackground.png"))
	{
		LOGGER->Log("LineLog", "Unable to load log background");
	}
	logBackgroundImage.setSmooth(true);
	logBackground.setTexture(logBackgroundImage);

	logBars.push_back(new LogBar());
	logBars[0]->load();
	logBars[0]->setPosition(0, 675);

	logBars.push_back(new LogBar());
	logBars[1]->load();
	logBars[1]->setPosition(0, 450);

	logBars.push_back(new LogBar());
	logBars[2]->load();
	logBars[2]->setPosition(0, 225);

	logBars.push_back(new LogBar());
	logBars[3]->load();
	logBars[3]->setPosition(0, 0);
}

LineLog::~LineLog()
{
	for (auto log : logBars)
	{
		if (log) delete log;
	}
}

void LineLog::addLogItem(std::string name, 
	std::string line, 
	std::string musicFile,
	std::vector<std::string> voiceFile, 
	std::string scriptFile, 
	unsigned int scriptFileSeekg, 
	std::set<std::string> flags)
{
	logItems.push_back(LineLogItem(name, line, voiceFile, musicFile, scriptFile, scriptFileSeekg, flags));
	
	// ensure logcount doesn't exceed max log items
	while (logItems.size() > MAX_LOGITEMS)
	{
		logItems.erase(logItems.begin());
	}
}

void LineLog::addLogItem(LineLogItem logItem)
{
	logItems.push_back(logItem);

	// ensure logcount doesn't exceed max log items
	while (logItems.size() > MAX_LOGITEMS)
	{
		logItems.erase(logItems.begin());
	}
}

std::string LineLog::getName(int i)
{
	if (i > logItems.size() - 1)
	{
		LOGGER->Log("LineLog", "Invalid index");
		return std::string();
	}

	return logItems[logItems.size() - 1 - i].name;
}

std::string LineLog::getLine(int i)
{
	if (i > logItems.size() - 1)
	{
		LOGGER->Log("LineLog", "Invalid index");
		return std::string();
	}

	return logItems[logItems.size() - 1 - i].line;
}

std::string LineLog::getMusicFile(int i)
{
	if (i > logItems.size() - 1)
	{
		LOGGER->Log("LineLog", "Invalid index");
		return std::string();
	}

	return logItems[logItems.size() - 1 - i].musicFile;
}

std::vector<std::string> LineLog::getVoiceFile(int i)
{
	if (i > logItems.size() - 1)
	{
		LOGGER->Log("LineLog", "Invalid index");
		return std::vector<std::string>();
	}

	return logItems[logItems.size() - 1 - i].voiceFile;
}

std::string LineLog::getScriptFile(int i)
{
	if (i > logItems.size() - 1)
	{
		LOGGER->Log("LineLog", "Invalid index");
		return std::string();
	}

	return logItems[logItems.size() - 1 - i].scriptFile;
}

unsigned int LineLog::getScriptFilePos(int i)
{
	if (i > logItems.size() - 1)
	{
		LOGGER->Log("LineLog", "Invalid index");
		return -1;
	}

	return logItems[logItems.size() - 1 - i].scriptFilePos;
}

std::set<std::string> LineLog::getFlags(int i)
{
	if (i > logItems.size() - 1)
	{
		LOGGER->Log("LineLog", "Invalid index");
		return std::set<std::string>();
	}

	return logItems[logItems.size() - 1 - i].flags;
}

bool LineLog::shouldJump(bool reset)
{
	bool temp = jumping;
	if (reset)
	{
		jumping = false;
	}
	return temp;
}

int LineLog::getJumpIdx() const
{
	return jumpIdx;
}

int LineLog::getBottomIdx() const
{
	return bottom;
}

int LineLog::getCurrentHoverIndex() const
{
	if (currentlyHovered < 0) return currentlyHovered;

	return bottom + currentlyHovered;
}

int LineLog::getLogCount() const
{
	return logItems.size();
}

bool LineLog::shouldDisplay() const
{
	return displaying;
}

void LineLog::displayLog(bool disp)
{
	displaying = disp;
}

void LineLog::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	if (displaying)
	{
		for (int i = 0; i < logBars.size(); i++)
		{
			currentlyHovered = -1;

			logBars[i]->handleInput(e, window);
			if (logBars[i]->isOnHover())
			{
				currentlyHovered = i;
			}
			if (logBars[i]->isClicked())
			{
				jumping = true;
				jumpIdx = getCurrentHoverIndex();
			}
		}

		switch (e.type)
		{
			case sf::Event::MouseButtonReleased:
			{
				if (e.mouseButton.button == sf::Mouse::Right)
				{
					bottom = 0;
					displaying = false;
				}					
				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				if (e.mouseWheelScroll.delta > 0)
				{
					bottom++;
					if (bottom > int(logItems.size()) - 4)
					{
						bottom = logItems.size() - 4;
					}

					if (bottom < 0) bottom = 0;
				}
				else if (e.mouseWheelScroll.delta < 0)
				{
					bottom--;
					if (bottom < 0)
					{
						bottom = 0;
						displaying = false;
					}
				}
				break;
			}
		}
	}
}

void LineLog::render(sf::RenderWindow & window)
{
	if (displaying)
	{
		window.draw(logBackground);
		
		for (auto logBar : logBars)
		{
			window.draw(*logBar);
		}
	}
}

void LineLog::update(float delta_t)
{
	if (displaying)
	{
		for (auto logBar : logBars)
		{
			logBar->update(delta_t);
		}

		for (int i = bottom; i < bottom + 4; i++)
		{

			if (i < logItems.size())
			{
				logBars[i - bottom]->setActive(true);
				logBars[i - bottom]->setName(logItems[logItems.size() - 1 - i ].name );
				logBars[i - bottom]->setLine(logItems[logItems.size() - 1 - i].line);
			}
			else
			{
				logBars[i - bottom]->setActive(false);
				logBars[i - bottom]->setName("");
				logBars[i - bottom]->setLine("");
			}
		}
	}
}
