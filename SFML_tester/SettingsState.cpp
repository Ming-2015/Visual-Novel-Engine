#include "SettingsState.h"

SettingsState::SettingsState()
{
	slider = Slider::createSettingsSlider(100.0f, 100.0f);
}

SettingsState::~SettingsState()
{
	if (slider != nullptr)
	{
		delete slider;
	}
}

void SettingsState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	slider->handleInput(e, window);
}

void SettingsState::render(sf::RenderWindow & window)
{
	window.draw(*slider);
}

void SettingsState::update(float delta_t)
{
	slider->update(delta_t);
}

void SettingsState::init()
{
	slider->load();
}
