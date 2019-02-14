#include "SettingsState.h"

SettingsState::SettingsState()
{
	myState = GameState::STATE_CONFIG;
	init();
}

SettingsState::~SettingsState()
{
	for (Slider * slider : sliders)
	{
		if (slider != nullptr) delete slider;
	}
}

void SettingsState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	for (Slider * slider : sliders)
	{
		if (slider != nullptr)
		{
			slider->handleInput(e, window);
		}
	}

	switch (e.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			if (texts[TEXT_SAVE].getGlobalBounds().contains(mousePosF))
			{
				CONFIG->masterVolume = sliders[SLIDER_MASTER]->getValue();
				CONFIG->bgmVolume = sliders[SLIDER_BGM]->getValue();
				CONFIG->sfxVolume = sliders[SLIDER_SFX]->getValue();
				CONFIG->voiceVolume = sliders[SLIDER_VOICE]->getValue();
				CONFIG->textWindowAlpha = sliders[SLIDER_ALPHA]->getValue();
				CONFIG->write("config.ini");

				nextState = GameState::STATE_BACK;
				shouldChangeState = true;
			}
			break;
		}
	}

}

void SettingsState::render(sf::RenderWindow & window)
{
	window.draw(configBackground);

	for (Slider * slider : sliders)
	{
		if (slider != nullptr)
		{
			window.draw(*slider);
		}
	}

	for (auto t : texts)
	{
		window.draw(t);
	}
}

void SettingsState::update(float delta_t)
{
	for (Slider * slider : sliders)
	{
		if (slider != nullptr)
		{
			slider->update(delta_t);
		}
	}
}

void SettingsState::init()
{
	if (!configTexture.loadFromFile("assets/ConfigurationsPage.png"))
		LOGGER->Log("MenuState", "Image not found: ConfigurationsPage.png");
	configBackground.setTexture(configTexture);

	sliders.push_back(Slider::createSettingsSlider(375.0f, 270.0f, CONFIG->masterVolume));
	sliders.push_back(Slider::createSettingsSlider(375.0f, 370.0f, CONFIG->bgmVolume));
	sliders.push_back(Slider::createSettingsSlider(375.0f, 470.0f, CONFIG->voiceVolume));
	sliders.push_back(Slider::createSettingsSlider(375.0f, 570.0f, CONFIG->sfxVolume));
	sliders.push_back(Slider::createSettingsSlider(375.0f, 670.0f, CONFIG->textWindowAlpha));

	if (!settingsFont.loadFromFile("assets/default.TTF"))
	{
		LOGGER->Log("SettingsState", "Unable to find default font");
	}

	texts.push_back(sf::Text("Master Volume", settingsFont, 30));
	texts.push_back(sf::Text("BGM Volume", settingsFont, 30));
	texts.push_back(sf::Text("Voice Volume", settingsFont, 30));
	texts.push_back(sf::Text("SFX Volume", settingsFont, 30));
	texts.push_back(sf::Text("Textbox Transparency", settingsFont, 22));
	texts.push_back(sf::Text("Save", settingsFont, 54));

	texts[TEXT_MASTER].setPosition(125, 270);
	texts[TEXT_BGM].setPosition(125, 370);
	texts[TEXT_VOICE].setPosition(125, 470);
	texts[TEXT_SFX].setPosition(125, 570);
	texts[TEXT_ALPHA].setPosition(125, 670);
	texts[TEXT_SAVE].setPosition(725, 780);

	for (Slider * slider : sliders)
	{
		if (slider != nullptr)
		{
			slider->load();
		}
	}
}

void SettingsState::cleanup()
{
}
