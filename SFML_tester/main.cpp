#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Visual Novel");

	sf::Texture texture;
	sf::Sprite textBox;
	if (!texture.loadFromFile("textBoxDemo.png"))
	{
		cout << "Image Load Failure";
	}

	textBox.setTexture(texture);
	textBox.setPosition(400, 400);
	cout << "Image Load Failure";
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(textBox);
		window.display();
	}

	

	return 0;
}