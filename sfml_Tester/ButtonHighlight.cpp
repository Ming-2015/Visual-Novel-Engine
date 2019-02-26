#include "ButtonHighlight.h"

void ButtonHighlight::init()
{
	//CHANGE PNGS
	if (!autoTexture.loadFromFile(GLOBAL->AssetRoot + "MainMenuPrompt2.png"))
	{
		LOGGER->Log("ButtonHighlight", "Image not found: MainMenuPrompt.png");
	}
	autoSprite.setTexture(autoTexture);
	sprites.push_back(autoSprite);

	if (!skipTexture.loadFromFile(GLOBAL->AssetRoot + "MainMenuPrompt2.png"))
	{
		LOGGER->Log("ButtonHighlight", "Image not found: MainMenuPrompt.png");
	}
	skipSprite.setTexture(autoTexture);
	sprites.push_back(skipSprite);

	autoButton = new MenuButton(GLOBAL->AssetRoot + "Auto72x88.png", "", "", 1230.0f, 898.0f, 0, 0, 0, 44, 72, 44);
	autoButton->load();
	buttons.push_back(autoButton);

	skipButton = new MenuButton(GLOBAL->AssetRoot + "Skip69x98.png", "", "", 1157.0f, 902.0f, 0, 0, 0, 49, 69, 49);
	skipButton->load();
	buttons.push_back(skipButton);

	autoHidden = true;
	skipHidden = true;
}

void ButtonHighlight::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	for (MenuButton* button : buttons)
	{
		button->handleInput(e, window);
	}

	if (autoButton->isClicked(true))
	{
		autoButtonClicked = true;
		autoHidden = true;
	}
	if (skipButton->isClicked(true))
	{
		skipButtonClicked = true;
		skipHidden = true;
	}
}

void ButtonHighlight::render(sf::RenderWindow & window)
{
	if (autoHidden == false)
	{
		window.draw(sprites[AUTO_BUTTON]);
		window.draw(*buttons[AUTO_BUTTON]);
	}
	if (skipHidden == false)
	{
		window.draw(sprites[SKIP_BUTTON]);
		window.draw(*buttons[SKIP_BUTTON]);
	}
}

void ButtonHighlight::update(float delta_t)
{
	for (MenuButton* button : buttons)
	{
		button->update(delta_t);
	}
}

void ButtonHighlight::cleanup()
{
	/*for (sf::Sprite sprite : sprites)
	{
		if (&sprite) delete &sprite;
	}
	sprites.clear();*/
	//Might not need this

	for (MenuButton* button : buttons)
	{
		if (button) delete button;
	}
	buttons.clear();
}

ButtonHighlight::ButtonHighlight()
{
	init();
}

ButtonHighlight::~ButtonHighlight()
{
	cleanup();
}