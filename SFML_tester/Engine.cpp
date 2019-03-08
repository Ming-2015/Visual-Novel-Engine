#include "Engine.h"

int Engine::start()
{
	// initialize configs
	CONFIG->parse("config.ini");

	window.create(sf::VideoMode(CONFIG->getWindowWidth(), CONFIG->getWindowHeight()), CONFIG->getTitle(), sf::Style::Default & ~sf::Style::Resize);
	window.setFramerateLimit(CONFIG->getFps());
	
	initFPSText();
	GLOBAL->windowPtr = &window;

	// change window size based on config data
	initWindowSize(window);

	Game game;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			game.handleInput(event, window);
		}

		// engine timings
		delta_t = (gameClock.getElapsedTime() - prevTime).asSeconds();
		fps = 1.0f / delta_t;
		prevTime = gameClock.getElapsedTime();

		// update game logics here
		game.update(delta_t);

		// render everything from scratch, so clear away the window first
		window.clear(sf::Color(0,0,0));

		// render here
		game.render(window);

		// display fps font
		if (displayFPS)
		{
			fpsStr = "FPS: " + to_string((int)fps);
			fpsText.setString(fpsStr);
			window.draw(fpsText);
		}

		window.display();
	}

	Logger::Cleanup();
	Config::Cleanup();
	Utility::CleanUp();
	SavedataUtility::CleanUp();

	return 0;
}

Engine::Engine()
{

}

void Engine::initFPSText()
{
	prevTime = sf::Time(sf::seconds(0));

	fpsStr = "FPS: ";
	fpsText = sf::Text();
	fpsFont = sf::Font();
	if (!fpsFont.loadFromFile(GLOBAL->UserInterfaceFont))
	{
		LOGGER->Log("Engine", "Unable to open dialogueFont");
	}

	fpsText.setFont(fpsFont);
	fpsText.setStyle(sf::Text::Regular);
	fpsText.setString(fpsStr);
	fpsText.setFillColor(sf::Color(255, 255, 255, 255));
	fpsText.setCharacterSize(24);
	fpsText.setPosition(0, 0);
}

void Engine::initWindowSize(sf::RenderWindow & window)
{

	// create an unresizable window
	float windowZoom = float(CONFIG->defaultWidth) / float(CONFIG->getWindowWidth());
	float aspectRatio = float(CONFIG->getWindowWidth()) / float(CONFIG->getWindowHeight());
	if (fabs(aspectRatio - CONFIG->defaultWidth / CONFIG->defaultHeight) > 0.01f)
	{
		std::string err;

		err = "Bad Aspect Ratio: " + to_string(aspectRatio) + " compared to " + to_string(16.f / 9.f);
		LOGGER->Log("Engine", err);
		err = "Window Width: " + to_string(CONFIG->getWindowWidth());
		LOGGER->Log("Engine", err);
		err = "Window Height: " + to_string(CONFIG->getWindowHeight());
		LOGGER->Log("Engine", err);
	}
	sf::View defView = window.getView();
	sf::View newView = defView;
	newView.zoom(windowZoom);
	newView.setCenter(CONFIG->defaultWidth / 2.0f, CONFIG->defaultHeight / 2.0f);
	window.setView(newView);
}
