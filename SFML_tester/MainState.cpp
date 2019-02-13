#include "MainState.h"
#include "Config.h"

void MainState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	switch (e.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			if (isFading)
			{
				LOGGER->Log("MainState", "Fading skipping, loading next image");
				isFading = false;
				fade->skip();
				if (!background.loadFromFile(scriptManager->getBackgroundFileName()))
				{
					LOGGER->Log("MenuState", "Unable to get Background Image");
				}
				displayBackground.setTexture(background);
			}
			else
			{
				scriptManager->readNextLine();
				character = 0;
				if (!scriptManager->getBackgroundChange())
				{
					//if (!background.loadFromFile(scriptManager->getBackgroundFileName()))
					//{
					//	LOGGER->Log("MenuState", "Unable to get Background Image");
					//}
					//displayBackground.setTexture(background);

					if (fade) delete fade;
					fade = new Fade(scriptManager->getBackgroundFileName(), 10, 10);
					isFading = true;
					fade->load();
					fade->start();

					std::string msg = scriptManager->getBackgroundFileName();
					LOGGER->Log("MainState", msg);
				}
				if (scriptManager->getTextboxChange()) {
					if (!textbox.loadFromFile(scriptManager->getTextboxFileName()))
						LOGGER->Log("MenuState", "Unable to get Textbox Image");
					displayTextbox.setTexture(textbox);
				}
			}
		}
		break;
	}
}

void MainState::render(sf::RenderWindow & window)
{
	
	window.draw(displayBackground);
	
	if (fade && isFading)
	{
		window.draw(*fade);
	}

	if (!isFading)
	{
		window.draw(displayTextbox);
		window.draw(displayNameStr);
		window.draw(displayTextStr);
	}
}

void MainState::update(float delta_t)
{
	if (!isFading)
	{
		if (clock.getElapsedTime().asMilliseconds() > ((float) 1.0f / (CONFIG->manualTextSpeed + 0.01) + 5.f) && character < scriptManager->getScriptLine().length())
		{
			clock.restart();
			character++;
			displayTextStr.setString(scriptManager->getScriptLine().substr(0, character));
			//if (displayTextStr.getGlobalBounds().intersects(displayTextbox.getGlobalBounds()))
			if (character != 0 && character%70 == 0)
			{
				scriptManager->addNewLineToPrevWord(character);
				displayTextStr.setString(scriptManager->getScriptLine().substr(0, ++character));
			}
		}
	}

	displayNameStr.setString(scriptManager->getDisplayName());

	if (scriptManager->eof())
	{
		shouldChangeState = true;
		nextState = GameState::STATE_MENU;
	}

	GLOBAL->MAIN_STATE_currentFile = scriptManager->getCurrentFileName();
	GLOBAL->MAIN_STATE_currentLineId = scriptManager->getCurrentLineId();

	if (fade && isFading)
	{
		fade->update(delta_t);
		if (fade->isDone())
		{
			LOGGER->Log("MainState", "Fading done, loading next image");
			isFading = false;
			if (!background.loadFromFile(scriptManager->getBackgroundFileName()))
			{
				LOGGER->Log("MenuState", "Unable to get Background Image");
			}
			displayBackground.setTexture(background);
		}
	}
}

void MainState::init()
{
	scriptManager->init();
	if (!background.loadFromFile(scriptManager->getBackgroundFileName()))
		LOGGER->Log("MainState", "Unable to get Background Image");
	displayBackground.setTexture(background);
	if (!textbox.loadFromFile(scriptManager->getTextboxFileName()))
		LOGGER->Log("MainState", "Unable to get Textbox Image");
	displayTextbox.setTexture(textbox);
	displayTextbox.setColor(sf::Color(255, 255, 255, 255 * CONFIG->textWindowAlpha));

	if (!displayNameFont.loadFromFile("assets/MATURASC.TTF"))
	{
		LOGGER->Log("MainState", "Cannot find font file 1");
	}
	else
	{
		displayNameColor = sf::Color(255, 255, 255);

		displayNameStr.setFont(displayNameFont);
		displayNameStr.setStyle(sf::Text::Bold | sf::Text::Underlined);
		displayNameStr.setString(scriptManager->getDisplayName());
		displayNameStr.setFillColor(displayNameColor);
		displayNameStr.setCharacterSize(32);
		displayNameStr.setPosition(230.0f, 643.0f);
	}

	if (!displayTextFont.loadFromFile("assets/default.TTF"))
	{
		LOGGER->Log("MainState", "Cannot find font file 2");
	}
	else
	{
		displayTextColor = sf::Color(255, 255, 255);

		displayTextStr.setFont(displayTextFont);
		displayTextStr.setStyle(sf::Text::Regular);
		displayTextStr.setFillColor(displayTextColor);
		displayTextStr.setCharacterSize(32);
		displayTextStr.setPosition(230.0f, 705.0f);
	}
}

const ScriptManager * MainState::getScriptManager()
{
	return scriptManager;
}

MainState::MainState(std::string filename, int lineId)
{
	scriptManager = new ScriptManager(filename, lineId);
	fade = nullptr;
}

MainState::~MainState()
{
	if (fade) delete fade;
	delete scriptManager;
}
