#include "LoadingScreenState.h"

void LoadingScreenState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
}

void LoadingScreenState::render(sf::RenderWindow & window)
{
	window.draw(background);
	window.draw(progressText);
}

void LoadingScreenState::update(float delta_t)
{
	float progress = RESOURCE->calcProgress();
	progressStr = "Current Progress: " + to_string(progress*100.f) + "%";
	progressText.setString(progressStr);

	timer += delta_t;
	if (timer >= 1.f && RESOURCE->doneLoading())
	{
		RESOURCE->joinAll();
		shouldChangeState = true;
		nextState = STATE_BACK;
	}
}

void LoadingScreenState::init()
{
	std::string loadingScreenPath = GLOBAL->AssetRoot + "loading.jpg";

	// add the resource and start loading them
	sf::Font* font = RESOURCE->getFont(GLOBAL->UserInterfaceFont);
	sf::Texture* tex = RESOURCE->getTexture(loadingScreenPath);

	// wait till the resouces are loaded (might take a short, minimal amount of time)
	RESOURCE->joinFont(GLOBAL->UserInterfaceFont);
	RESOURCE->joinTexture(loadingScreenPath);

	// apply the resources
	background.setTexture(*tex);

	progressText.setFont(*font);
	progressText.setFillColor(sf::Color::White);
	progressText.setOutlineColor(sf::Color::Black);
	progressText.setCharacterSize(42);
	progressText.setOutlineThickness(4);
	progressText.setPosition(880.0f, 790.0f);

	// Start loading all the assets added previously
	RESOURCE->startLoading();
}

void LoadingScreenState::cleanup()
{
}

LoadingScreenState::LoadingScreenState()
{
	myState = GameState::STATE_LOADING_SCREEN;
	init();
}
