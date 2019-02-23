#include "SettingsState.h"

SettingsState::SettingsState()
{
	myState = GameState::STATE_CONFIG;
	init();
}

SettingsState::~SettingsState()
{

}

void SettingsState::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	for (MenuButton * button : zeroButtons)
	{
		if (button != nullptr)
		{
			button->handleInput(e, window);
		}
	}

	for (MenuButton * button : hundredButtons)
	{
		if (button != nullptr)
		{
			button->handleInput(e, window);
		}
	}

	for (int i = 0; i < zeroButtons.size(); i++)
	{
		if (zeroButtons[i]->isClicked(true))
		{
			sliders[i]->setValue(0.0);
		}
	}
	for (int i = 0; i < hundredButtons.size(); i++)
	{
		if (hundredButtons[i]->isClicked(true))
		{
			sliders[i]->setValue(1.0);
		}
	}

	for (Slider * slider : sliders)
	{
		if (slider != nullptr)
		{
			slider->handleInput(e, window);
		}
	}

	switch (e.type)
	{
		case sf::Event::MouseButtonReleased:
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
				LOGGER->Log("SettingsState", "Returning to Main Menu");
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

	for (MenuButton * button : zeroButtons)
	{
		if (button != nullptr)
		{
			window.draw(*button);
		}
	}

	for (MenuButton * button : hundredButtons)
	{
		if (button != nullptr)
		{
			window.draw(*button);
		}
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

	for (MenuButton * buttons : zeroButtons)
	{
		if (buttons != nullptr)
		{
			buttons->update(delta_t);
		}
	}

	for (MenuButton * buttons : hundredButtons)
	{
		if (buttons != nullptr)
		{
			buttons->update(delta_t);
		}
	}
}

void SettingsState::init()
{
	if (!configTexture.loadFromFile("assets/ConfigurationsPage.png"))
		LOGGER->Log("MenuState", "Image not found: ConfigurationsPage.png");
	configBackground.setTexture(configTexture);

	zeroButtons.push_back(new MenuButton("assets/scrollBarZero.png", "", "", 400.0f, 290.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[0]->load();
	zeroButtons.push_back(new MenuButton("assets/scrollBarZero.png", "", "", 400.0f, 390.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[1]->load();
	zeroButtons.push_back(new MenuButton("assets/scrollBarZero.png", "", "", 400.0f, 490.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[2]->load();
	zeroButtons.push_back(new MenuButton("assets/scrollBarZero.png", "", "", 400.0f, 590.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[3]->load();
	zeroButtons.push_back(new MenuButton("assets/scrollBarZero.png", "", "", 400.0f, 690.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[4]->load();

	hundredButtons.push_back(new MenuButton("assets/scrollBarHundred.png", "", "", 715.0f, 290.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[0]->load();
	hundredButtons.push_back(new MenuButton("assets/scrollBarHundred.png", "", "", 715.0f, 390.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[1]->load();
	hundredButtons.push_back(new MenuButton("assets/scrollBarHundred.png", "", "", 715.0f, 490.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[2]->load();
	hundredButtons.push_back(new MenuButton("assets/scrollBarHundred.png", "", "", 715.0f, 590.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[3]->load();
	hundredButtons.push_back(new MenuButton("assets/scrollBarHundred.png", "", "", 715.0f, 690.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[4]->load();


	sliders.push_back(Slider::createSettingsSlider(435.0f, 280.0f, CONFIG->masterVolume));
	sliders.push_back(Slider::createSettingsSlider(435.0f, 380.0f, CONFIG->bgmVolume));
	sliders.push_back(Slider::createSettingsSlider(435.0f, 480.0f, CONFIG->voiceVolume));
	sliders.push_back(Slider::createSettingsSlider(435.0f, 580.0f, CONFIG->sfxVolume));
	sliders.push_back(Slider::createSettingsSlider(435.0f, 680.0f, CONFIG->textWindowAlpha));

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
	for (Slider * slider : sliders)
	{
		if (slider != nullptr) delete slider;
	}
}
