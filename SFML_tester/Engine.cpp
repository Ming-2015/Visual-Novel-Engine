#include "Engine.h"

int Engine::start()
{
	// initialize configs
	CONFIG->parse("config.ini");

	// create an unresizable window
	window.create(sf::VideoMode(CONFIG->getWindowWidth(), CONFIG->getWindowHeight()), CONFIG->getTitle(), sf::Style::Default & ~sf::Style::Resize);
	window.setFramerateLimit(CONFIG->getFps());

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

			game.handleInput(event);
		}

		// update game logics here
		game.update();

		// render everything from scratch, so clear away the window first
		window.clear();

		// render here
		game.render(window);
		window.display();
	}
	return 0;
}

Engine::Engine()
{

}
