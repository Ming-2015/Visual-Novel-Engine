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
	for (DarkenButton * button : staticDButtons)
	{
		if (button != nullptr)
		{
			button->handleInput(e, window);
		}
	}

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

	if (currentPage == VG)
	{
		for (RadioButton * radioButton : displayOptionButtons)
		{
			if (radioButton != nullptr)
			{
				radioButton->handleInput(e, window);
			}
		}
	}

	if (currentPage == DG)
	{
		for (CheckBox* b : gameFeatures)
		{
			if (b != nullptr)
			{
				b->handleInput(e, window);
			}
		}
	}

	switch (e.type)
	{
		case sf::Event::MouseButtonReleased:
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);
				if (staticDButtons[SAVE]->isClicked(true))
				{
					CONFIG->manualTextSpeed = sliders[SLIDER_MANUAL_SPEED]->getValue();
					CONFIG->autoTextSpeed = sliders[SLIDER_AUTO_SPEED]->getValue();
					CONFIG->autoTextWaitTime = sliders[SLIDER_AUTO_WAIT_TIME]->getValue();
					CONFIG->textWindowAlpha = sliders[SLIDER_ALPHA]->getValue();
					CONFIG->masterVolume = sliders[SLIDER_MASTER]->getValue();
					CONFIG->bgmVolume = sliders[SLIDER_BGM]->getValue();
					CONFIG->sfxVolume = sliders[SLIDER_SFX]->getValue();
					CONFIG->voiceVolume = sliders[SLIDER_VOICE]->getValue();
					
					
					// save previous option to see if there's a need to reset window
					FullscreenOpts prevFullscreenOps = CONFIG->enableFullscreen;

					// check which option is being selected
					CONFIG->enableFullscreen = static_cast<FullscreenOpts>
						(fullScreenButton->getSelected() ? FullscreenOpts::fullscreen : 
						borderlessButton->getSelected() ? FullscreenOpts::borderless : 
							FullscreenOpts::windowed);
					
					if (CONFIG->enableFullscreen != prevFullscreenOps)
					{
						CONFIG->resetWindowSize(window);
					}

					CONFIG->write("config.ini");

					nextState = GameState::STATE_BACK;
					shouldChangeState = true;
					LOGGER->Log("SettingsState", "Returning to Main Menu");
				}
				break;
			}
			else if (e.mouseButton.button == sf::Mouse::Right)
			{
				nextState = GameState::STATE_BACK;
				shouldChangeState = true;
				LOGGER->Log("SettingsState", "Returning to Previous State");
			}
		}
	}
}

void SettingsState::render(sf::RenderWindow & window)
{
	window.draw(configBackground);
	window.draw(DGBackground);
	window.draw(VGBackground);

	for (DarkenButton * button : staticDButtons)
	{
		if (button != nullptr)
		{
			window.draw(*button);
		}
	}

	for (Slider * slider : sliders)
	{
		if (slider != nullptr)
		{
			window.draw(*slider);
		}
	}

	/*for (auto t : texts)
	{
		window.draw(t);
	}*/

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

	if (currentPage == VG)
	{
		for (RadioButton * radioButton : displayOptionButtons)
		{
			if (radioButton != nullptr)
			{
				window.draw(*radioButton);
			}
		}
	}

	if (currentPage == DG)
	{
		for (CheckBox* b : gameFeatures)
		{
			if (b != nullptr)
			{
				window.draw(*b);
			}
		}
	}
}

void SettingsState::update(float delta_t)
{
	for (DarkenButton * button : staticDButtons)
	{
		if (button != nullptr)
		{
			button->update(delta_t);
		}
	}

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

	if (currentPage == VG)
	{
		for (RadioButton * radioButton : displayOptionButtons)
		{
			if (radioButton != nullptr)
			{
				radioButton->update(delta_t);
			}
		}
	}

	if (currentPage == DG)
	{
		for (CheckBox* b : gameFeatures)
		{
			if (b != nullptr)
			{
				b->update(delta_t);
			}
		}
	}
}

void SettingsState::init()
{
	if (!configTexture.loadFromFile(GLOBAL->AssetRoot + "ConfigurationsPage.png"))
		LOGGER->Log("MenuState", "Image not found: ConfigurationsPage.png");
	configTexture.setSmooth(true);
	configBackground.setTexture(configTexture);

	staticDButtons.push_back(new DarkenButton(GLOBAL->AssetRoot + "DGButton.png", "", "", 195.0f, 295.0f, 0, 0, 0, 0, 318, 189));
	staticDButtons[DG]->load();
	staticDButtons.push_back(new DarkenButton(GLOBAL->AssetRoot + "VGButton.png", "", "", 195.0f, 520.0f, 0, 0, 0, 0, 318, 189));
	staticDButtons[VG]->load();
	staticDButtons.push_back(new DarkenButton(GLOBAL->AssetRoot + "ConfigDefault.png", "", "", 200.0f, 680.0f, 0, 0, 0, 0, 188, 81));
	staticDButtons[DEFAULT]->load();
	staticDButtons.push_back(new DarkenButton(GLOBAL->AssetRoot + "ConfigReturn.png", "", "", 200.0f, 760.0f, 0, 0, 0, 0, 188, 81));
	staticDButtons[RETURN]->load();
	staticDButtons.push_back(new DarkenButton(GLOBAL->AssetRoot + "ConfigSave.png", "", "", 200.0f, 840.0f, 0, 0, 0, 0, 188, 81));
	staticDButtons[SAVE]->load();

	//THESE ASSETS ARE FOR DG!!!!           REMOVE THE + 900 TO SHOW
	if (!DGTexture.loadFromFile(GLOBAL->AssetRoot + "DG_bg.png"))
		LOGGER->Log("MenuState", "Image not found: DG_bg.png");
	DGTexture.setSmooth(true);
	DGBackground.setTexture(DGTexture);
	DGBackground.setPosition(435, 30 + 900);

	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 570.0f + 900, 0, 0, 0, 0, 64, 29));
	zeroButtons[0]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 645.0f + 900, 0, 0, 0, 0, 64, 29));
	zeroButtons[1]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 720.0f + 900, 0, 0, 0, 0, 64, 29));
	zeroButtons[2]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 795.0f + 900, 0, 0, 0, 0, 64, 29));
	zeroButtons[3]->load();


	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 570.0f + 900, 0, 0, 0, 0, 64, 29));
	hundredButtons[0]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 645.0f + 900, 0, 0, 0, 0, 64, 29));
	hundredButtons[1]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 720.0f + 900, 0, 0, 0, 0, 64, 29));
	hundredButtons[2]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 795.0f + 900, 0, 0, 0, 0, 64, 29));
	hundredButtons[3]->load();



	sliders.push_back(Slider::createSettingsSlider(975.0f, 560.0f + 900, CONFIG->manualTextSpeed));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 635.0f + 900, CONFIG->autoTextSpeed));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 710.0f + 900, CONFIG->autoTextWaitTime));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 785.0f + 900, CONFIG->textWindowAlpha));

	//THESE ASSETS ARE FOR VG!!!!!
	if (!VGTexture.loadFromFile(GLOBAL->AssetRoot + "VG_bg.png"))
		LOGGER->Log("MenuState", "Image not found: VG_bg.png");
	VGTexture.setSmooth(true);
	VGBackground.setTexture(VGTexture);
	VGBackground.setPosition(435, 30);

	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 495.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[4]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 595.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[5]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 695.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[6]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 795.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[7]->load();


	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 495.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[4]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 595.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[5]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 695.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[6]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 795.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[7]->load();



	sliders.push_back(Slider::createSettingsSlider(975.0f, 485.0f, CONFIG->masterVolume));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 585.0f, CONFIG->bgmVolume));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 685.0f, CONFIG->voiceVolume));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 785.0f, CONFIG->sfxVolume));


	for (Slider * slider : sliders)
	{
		if (slider != nullptr)
		{
			slider->load();
		}
	}

	
	if (!settingsFont.loadFromFile(GLOBAL->UserInterfaceFont))
	{
		LOGGER->Log("SettingsState", "Unable to find default font");
	}


	//Change to PNGS, but still push back
	/*texts.push_back(sf::Text("Master Volume", settingsFont, 30));
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
	texts[TEXT_SAVE].setPosition(725, 780);*/

	// loading the display option radio buttons
	displayOptionButtons.push_back(windowedButton = new RadioButton(GLOBAL->AssetRoot + "windowedRB.png", 490, 200));
	displayOptionButtons.push_back(fullScreenButton = new RadioButton(GLOBAL->AssetRoot + "fullscreenRB.png", 750, 200));
	displayOptionButtons.push_back(borderlessButton = new RadioButton(GLOBAL->AssetRoot + "borderlessFullRB.png", 980, 200));
	for (auto b : displayOptionButtons)
	{
		b->load();
		b->setOtherRadioButtons(displayOptionButtons);
		b->addOffset(10, 0);
	}
	
	if (CONFIG->enableFullscreen == FullscreenOpts::fullscreen)
	{
		fullScreenButton->setSelected(true);
	}
	else if (CONFIG->enableFullscreen == FullscreenOpts::windowed)
	{
		windowedButton->setSelected(true);
	}
	else if (CONFIG->enableFullscreen == FullscreenOpts::borderless)
	{
		borderlessButton->setSelected(true);
	}

	// loading the game feature check boxes
	gameFeatures.push_back(skipUnreadText = new CheckBox(GLOBAL->AssetRoot + "SkipUnreadCB.png", 490, 170));
	gameFeatures.push_back(stopSkippingAtChoice = new CheckBox(GLOBAL->AssetRoot + "StopAtChoiceCB.png", 980, 170));
	gameFeatures.push_back(stopVoiceAtNewLine = new CheckBox(GLOBAL->AssetRoot + "SkipVoiceCB.png", 490, 240));
	
	for (CheckBox* b : gameFeatures)
	{
		if (b != nullptr)
		{
			b->load();
			b->addOffset(15, 5);
		}
	}

	skipUnreadText->setSelected(CONFIG->skipUnreadText);
	stopSkippingAtChoice->setSelected(CONFIG->stopSkippingAtChoice);
	stopVoiceAtNewLine->setSelected(CONFIG->stopVoiceNextLine);

}

void SettingsState::cleanup()
{
	for (Slider * slider : sliders)
	{
		if (slider != nullptr) delete slider;
	}

	for (MenuButton * b : zeroButtons)
	{
		if (b != nullptr) delete b;
	}

	for (MenuButton* b : hundredButtons)
	{
		if (b != nullptr) delete b;
	}

	for (DarkenButton* b : staticDButtons)
	{
		if (b != nullptr) delete b;
	}

	for (RadioButton* b : displayOptionButtons)
	{
		if (b != nullptr) delete b;
	}

	for (CheckBox* b : gameFeatures)
	{
		if (b != nullptr) delete b;
	}
}
