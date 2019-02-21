#include "Engine.h"

int Engine::start()
{
	// initialize configs
	CONFIG->parse("config.ini");

	// create an unresizable window
	window.create(sf::VideoMode(CONFIG->getWindowWidth(), CONFIG->getWindowHeight()), CONFIG->getTitle(), sf::Style::Default & ~sf::Style::Resize);
	window.setFramerateLimit(CONFIG->getFps());
	
	initFPSText();

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
	if (!fpsFont.loadFromFile("assets/default.ttf"))
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
