#include "ConfirmationPrompt.h"

void ConfirmationPrompt::init()
{
	if (!boxTexture.loadFromFile(GLOBAL->AssetRoot + "MainMenuPrompt2.png"))
	{
		LOGGER->Log("ConfirmationPrompt", "Image not found: MainMenuPrompt.png");
	}
	boxSprite.setTexture(boxTexture);

	noButton = new MainButton(GLOBAL->AssetRoot + "no78x106.png", "", "", 910.0f, 490.0f, 0, 0, 0, 0, 78, 53, 0, 53, 78, 53);
	noButton->load();
	buttons.push_back(noButton);

	yesButton = new MainButton(GLOBAL->AssetRoot + "yes92x108.png", "", "", 710.0f, 490.0f, 0, 0, 0, 0, 92, 54, 0, 54, 92, 54);
	yesButton->load();
	buttons.push_back(yesButton);

	isHidden = true;

	if (!font.loadFromFile(GLOBAL->ConfirmationPromptFont))
	{
		LOGGER->Log("Confirmation Prompt", "unable to load Alice.ttf font");
	}
	text.setFont(font);

	text.setStyle(sf::Text::Bold);
	text.setFillColor( sf::Color(255,255,255,255) );
	text.setOutlineColor( sf::Color( 50,50,50,255 ) );
	text.setOutlineThickness(3);
	text.setCharacterSize(64);
	text.setString(prompt);

	text.setPosition(800, 290);
	text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
}

void ConfirmationPrompt::handleInput(sf::Event & e, sf::RenderWindow & window)
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

void ConfirmationPrompt::render(sf::RenderWindow & window)
{
	window.draw(boxSprite);
	for (MainButton* button : buttons)
	{
		window.draw(*button);
	}
	window.draw(text); 
}

void ConfirmationPrompt::update(float delta_t)
{
	for (MainButton* button : buttons)
	{
		button->update(delta_t);
	}
}

void ConfirmationPrompt::cleanup()
{
	for (MainButton* button : buttons)
	{
		if (button) delete button;
	}
	buttons.clear();
}

ConfirmationPrompt::ConfirmationPrompt(std::string prompt, std::string title)
	:prompt(prompt), title(title)
{
	init();
}

ConfirmationPrompt::~ConfirmationPrompt()
{
	cleanup();
}

bool ConfirmationPrompt::shouldBeHidden() const
{
	return isHidden;
}

void ConfirmationPrompt::hidePrompt(bool hide)
{
	isHidden = hide;
}

bool ConfirmationPrompt::isNoButtonClicked(bool reset)
{
	bool tmp = noButtonClicked;
	if (reset) noButtonClicked = false;
	return tmp;
}

bool ConfirmationPrompt::isYesButtonClicked(bool reset)
{
	bool tmp = yesButtonClicked;
	if (reset) yesButtonClicked = false;
	return tmp;
}
