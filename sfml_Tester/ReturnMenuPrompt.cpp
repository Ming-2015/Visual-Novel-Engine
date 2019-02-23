#include "ReturnMenuPrompt.h"

void ReturnMenuPrompt::init()
{
	if (!boxTexture.loadFromFile(GLOBAL->AssetRoot + "MainMenuPrompt2.png"))
	{
		LOGGER->Log("ReturnMenuPrompt", "Image not found: MainMenuPrompt.png");
	}
	boxSprite.setTexture(boxTexture);

	noButton = new MainButton(GLOBAL->AssetRoot + "no78x106.png", "", "", 910.0f, 490.0f, 0, 0, 0, 0, 78, 53, 0, 53, 78, 53);
	noButton->load();
	buttons.push_back(noButton);

	yesButton = new MainButton(GLOBAL->AssetRoot + "yes92x108.png", "", "", 710.0f, 490.0f, 0, 0, 0, 0, 92, 54, 0, 54, 92, 54);
	yesButton->load();
	buttons.push_back(yesButton);

	isHidden = true;
}

void ReturnMenuPrompt::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	for (MainButton* button : buttons)
	{
		button->handleInput(e, window);
	}

	if (noButton->isClicked(true))
	{
		noButtonClicked = true;
	}
	if (yesButton->isClicked(true))
	{
		yesButtonClicked = true;
	}
}

void ReturnMenuPrompt::render(sf::RenderWindow & window)
{
	window.draw(boxSprite);
	for (MainButton* button : buttons)
	{
		window.draw(*button);
	}
}

void ReturnMenuPrompt::update(float delta_t)
{
	noButton->update(delta_t);
	yesButton->update(delta_t);
}

void ReturnMenuPrompt::cleanup()
{

}

ReturnMenuPrompt::ReturnMenuPrompt()
{
	init();
}

ReturnMenuPrompt::~ReturnMenuPrompt()
{

}