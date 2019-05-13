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

	for (RadioButton * radioButton : displayOptionButtons)
	{
		if (radioButton != nullptr)
		{
			radioButton->handleInput(e, window);
		}
	}

	for (CheckBox* b : gameFeatures)
	{
		if (b != nullptr)
		{
			b->handleInput(e, window);
		}
	}
	if (fontSelectDropbox) fontSelectDropbox->handleInput(e, window);

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

					// check which option is being clicked
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
				else if (staticDButtons[RETURN]->isClicked(true))
				{
					nextState = GameState::STATE_BACK;
					shouldChangeState = true;
					LOGGER->Log("SettingsState", "Returning to Previous State");
				}
				else if (staticDButtons[DEFAULT]->isClicked(true))
				{
					CONFIG->manualTextSpeed = sliders[SLIDER_MANUAL_SPEED]->setValue(0.7);
					CONFIG->autoTextSpeed = sliders[SLIDER_AUTO_SPEED]->setValue(0.5);
					CONFIG->autoTextWaitTime = sliders[SLIDER_AUTO_WAIT_TIME]->setValue(0.18);
					CONFIG->textWindowAlpha = sliders[SLIDER_ALPHA]->setValue(0.80);
					CONFIG->masterVolume = sliders[SLIDER_MASTER]->setValue(0.7);
					CONFIG->bgmVolume = sliders[SLIDER_BGM]->setValue(0.7);
					CONFIG->sfxVolume = sliders[SLIDER_SFX]->setValue(0.7);
					CONFIG->voiceVolume = sliders[SLIDER_VOICE]->setValue(0.7);
				}
				else if (staticDButtons[DG]->isClicked(true))
				{
					currentPage = DG;
				}
				else if (staticDButtons[VG]->isClicked(true))
				{
					currentPage = VG;
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

	for (RadioButton * radioButton : displayOptionButtons)
	{
		if (radioButton != nullptr)
		{
			window.draw(*radioButton);
		}
	}

	for (CheckBox* b : gameFeatures)
	{
		if (b != nullptr)
		{
			window.draw(*b);
		}
	}
	if (fontSelectDropbox) window.draw(*fontSelectDropbox);
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

	for (RadioButton * radioButton : displayOptionButtons)
	{
		if (radioButton != nullptr)
		{
			radioButton->update(delta_t);
		}
	}

	for (CheckBox* b : gameFeatures)
	{
		if (b != nullptr)
		{
			b->update(delta_t);
		}
	}
	if (fontSelectDropbox) fontSelectDropbox->update(delta_t);

	if (currentPage == VG)
	{
		if (currentDynamicOffset == 900)
		{
			currentDynamicOffset = 0;
		}
		if (clock.getElapsedTime().asMilliseconds() > 1.0f)
		{
			if (currentDynamicOffset > -900.0f)
			{
				//Iterator
				currentDynamicOffset -= 25;

				//DG Elements
				DGBackground.move(sf::Vector2f(0.0f, -25));
				fontSelectDropbox->move(0, -25);

				for (int i = 0; i < 3; i++)
				{
					displayOptionButtons[i]->addOffset(0, -25);
				}

				for (int i = 0; i < 4; i++)
				{
					zeroButtons[i]->move(0, -25);
				}

				for (int i = 0; i < 4; i++)
				{
					hundredButtons[i]->move(0, -25);
				}

				for (int i = 0; i < 4; i++)
				{
					sliders[i]->move(0, -25);
				}

				//VG Elements
				VGBackground.move(sf::Vector2f(0.0f, -25));

				for (int i = 0; i < 3; i++)
				{
					gameFeatures[i]->addOffset(0, -25);
				}

				for (int i = 4; i < 8; i++)
				{
					zeroButtons[i]->move(0, -25);
				}

				for (int i = 4; i < 8; i++)
				{
					hundredButtons[i]->move(0, -25);
				}

				for (int i = 4; i < 8; i++)
				{
					sliders[i]->move(0, -25);
				}
			}
			else if (currentDynamicOffset <= -900.0f)
			{
				currentDynamicOffset = -900;
			}
			clock.restart();
		}
	}
	else if (currentPage == DG)
	{
		if (currentDynamicOffset == -900)
		{
			currentDynamicOffset = 0;
		}
		if (clock.getElapsedTime().asMilliseconds() > 1.0f)
		{
			if (currentDynamicOffset < 900.0f)
			{
				//Iterator
				currentDynamicOffset += 25;

				//DG Elements
				DGBackground.move(sf::Vector2f(0.0f, 25));
				fontSelectDropbox->move(0, 25);

				for (int i = 0; i < 3; i++)
				{
					displayOptionButtons[i]->addOffset(0, 25);
				}

				for (int i = 0; i < 4; i++)
				{
					zeroButtons[i]->move(0, 25);
				}
				
				for (int i = 0; i < 4; i++)
				{
					hundredButtons[i]->move(0, 25);
				}

				for (int i = 0; i < 4; i++)
				{
					sliders[i]->move(0, 25);
				}

				//VG Elements
				VGBackground.move(sf::Vector2f(0.0f, 25));

				for (int i = 0; i < 3; i++)
				{
					gameFeatures[i]->addOffset(0, 25);
				}

				for (int i = 4; i < 8; i++)
				{
					zeroButtons[i]->move(0, 25);
				}

				for (int i = 4; i < 8; i++)
				{
					hundredButtons[i]->move(0, 25);
				}

				for (int i = 4; i < 8; i++)
				{
					sliders[i]->move(0, 25);
				}
			}
			else if (currentDynamicOffset >= 900.0f)
			{
				currentDynamicOffset = 900;
			}
			clock.restart();
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
	DGBackground.setPosition(435, 30);

	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 570.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[0]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 645.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[1]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 720.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[2]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 795.0f, 0, 0, 0, 0, 64, 29));
	zeroButtons[3]->load();


	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 570.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[0]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 645.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[1]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 720.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[2]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 795.0f, 0, 0, 0, 0, 64, 29));
	hundredButtons[3]->load();



	sliders.push_back(Slider::createSettingsSlider(975.0f, 560.0f, CONFIG->manualTextSpeed));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 635.0f, CONFIG->autoTextSpeed));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 710.0f, CONFIG->autoTextWaitTime));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 785.0f, CONFIG->textWindowAlpha));

	//THESE ASSETS ARE FOR VG!!!!!
	if (!VGTexture.loadFromFile(GLOBAL->AssetRoot + "VG_bg.png"))
		LOGGER->Log("MenuState", "Image not found: VG_bg.png");
	VGTexture.setSmooth(true);
	VGBackground.setTexture(VGTexture);
	VGBackground.setPosition(435, 30 + 900);

	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 495.0f + 900, 0, 0, 0, 0, 64, 29));
	zeroButtons[4]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 595.0f + 900, 0, 0, 0, 0, 64, 29));
	zeroButtons[5]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 695.0f + 900, 0, 0, 0, 0, 64, 29));
	zeroButtons[6]->load();
	zeroButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarZero.png", "", "", 940.0f, 795.0f + 900, 0, 0, 0, 0, 64, 29));
	zeroButtons[7]->load();


	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 495.0f + 900, 0, 0, 0, 0, 64, 29));
	hundredButtons[4]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 595.0f + 900, 0, 0, 0, 0, 64, 29));
	hundredButtons[5]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 695.0f + 900, 0, 0, 0, 0, 64, 29));
	hundredButtons[6]->load();
	hundredButtons.push_back(new MenuButton(GLOBAL->AssetRoot + "scrollBarHundred.png", "", "", 1255.0f, 795.0f + 900, 0, 0, 0, 0, 64, 29));
	hundredButtons[7]->load();



	sliders.push_back(Slider::createSettingsSlider(975.0f, 485.0f + 900, CONFIG->masterVolume));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 585.0f + 900, CONFIG->bgmVolume));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 685.0f + 900, CONFIG->voiceVolume));
	sliders.push_back(Slider::createSettingsSlider(975.0f, 785.0f + 900, CONFIG->sfxVolume));


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
	gameFeatures.push_back(skipUnreadText = new CheckBox(GLOBAL->AssetRoot + "SkipUnreadCB.png", 470, 170 + 900));
	gameFeatures.push_back(stopSkippingAtChoice = new CheckBox(GLOBAL->AssetRoot + "StopAtChoiceCB.png", 950, 170 + 900));
	gameFeatures.push_back(stopVoiceAtNewLine = new CheckBox(GLOBAL->AssetRoot + "SkipVoiceCB.png", 470, 240 + 900));
	
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

	// for the drop box
	fontSelectDropbox = new Dropbox(4, 910, 470);
	fontSelectDropbox->load();
	fontSelectDropbox->setOffset(30, 5);

	fontSelectDropbox->setText(0, "Cambria0");
	fontSelectDropbox->setText(1, "Cambria1");
	fontSelectDropbox->setText(2, "Cambria2");
	fontSelectDropbox->setText(3, "Cambria3");
	fontSelectDropbox->setText(4, "Cambria4");

	fontSelectDropbox->setFont(0, GLOBAL->UserInterfaceFont);
	fontSelectDropbox->setFont(1, GLOBAL->UserInterfaceFont);
	fontSelectDropbox->setFont(2, GLOBAL->UserInterfaceFont);
	fontSelectDropbox->setFont(3, GLOBAL->UserInterfaceFont);
	fontSelectDropbox->setFont(4, GLOBAL->UserInterfaceFont);

	fontSelectDropbox->setSelected(0);

	// set the duration of the animation: smaller = faster, 0 = instant
	fontSelectDropbox->setAnimationDuration(0.2f);
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

	if (fontSelectDropbox) delete fontSelectDropbox;
}
