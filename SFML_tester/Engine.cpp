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

		// update game logics here
		delta_t = (gameClock.getElapsedTime() - prevTime).asSeconds();
		prevTime = gameClock.getElapsedTime();
		game.update(delta_t);

		// render everything from scratch, so clear away the window first
		window.clear();

		// render here
		game.render(window);
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