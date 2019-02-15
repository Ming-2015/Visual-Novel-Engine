#include "Engine.h"

int Engine::start()
{
	// initialize configs
	CONFIG->parse("config.ini");

	// create an unresizable window
	window.create(sf::VideoMode(CONFIG->getWindowWidth(), CONFIG->getWindowHeight()), CONFIG->getTitle(), sf::Style::Default & ~sf::Style::Resize);
	window.setFramerateLimit(CONFIG->getFps());

	Game game;
	sf::Clock gameClock;
	sf::Time prevTime = sf::Time(sf::seconds(0));
	float delta_t;
	float fps;

	std::string fpsStr = "FPS: ";
	sf::Text fpsText = sf::Text();
	sf::Font fpsFont = sf::Font();
	if (!fpsFont.loadFromFile("assets/default.ttf"))
	{
		LOGGER->Log("Engine", "Unable to open font");
	}

	fpsText.setFont(fpsFont);
	fpsText.setStyle(sf::Text::Regular);
	fpsText.setString(fpsStr);
	fpsText.setFillColor(sf::Color(255,255,255,255));
	fpsText.setCharacterSize(24);
	fpsText.setPosition(0,0);

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
		window.clear(sf::Color(120,200,100));

		// render here
		game.render(window);

		// display fps font
		fpsStr = "FPS: " + to_string((int)fps);
		fpsText.setString(fpsStr);
		window.draw(fpsText);

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