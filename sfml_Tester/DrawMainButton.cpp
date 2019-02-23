#include "DrawMainButton.h"

void DrawMainButton::init()
{
	configButton = new MainButton("assets/Config55x100.png", "", "", 1574.0f, 838.0f, 0, 0, 0, 0, 55, 50, 0, 50, 55, 100);
	configButton->load();
	buttons.push_back(configButton);

	exitButton = new MainButton("assets/Exit55x112.png", "", "", 1575.0f, 903.0f, 0, 0, 0, 0, 55, 56, 0, 56, 55, 112);
	exitButton->load();
	buttons.push_back(exitButton);

	QLButton = new MainButton("assets/QuickLoad64x104.png", "", "", 1507.0f, 900.0f, 0, 0, 0, 0, 64, 52, 0, 52, 64, 104);
	QLButton->load();
	buttons.push_back(QLButton);

	loadButton = new MainButton("assets/Load63x104.png", "", "", 1437.0f, 900.0f, 0, 0, 0, 0, 63, 52, 0, 52, 63, 104);
	loadButton->load();
	buttons.push_back(loadButton);

	QSButton = new MainButton("assets/QuickSave53x106.png", "", "", 1367.0f, 900.0f, 0, 0, 0, 0, 53, 53, 0, 53, 53, 106);
	QSButton->load();
	buttons.push_back(QSButton);

	saveButton = new MainButton("assets/Save53x102.png", "", "", 1302.0f, 900.0f, 0, 0, 0, 0, 53, 51, 0, 51, 53, 102);
	saveButton->load();
	buttons.push_back(saveButton);

	autoButton = new MainButton("assets/Auto72x88.png", "", "", 1230.0f, 898.0f, 0, 0, 0, 0, 72, 44, 0, 44, 72, 88);
	autoButton->load();
	buttons.push_back(autoButton);

	skipButton = new MainButton("assets/Skip69x98.png", "", "", 1157.0f, 902.0f, 0, 0, 0, 0, 69, 49, 0, 49, 69, 98);
	skipButton->load();
	buttons.push_back(skipButton);
}

void DrawMainButton::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	for (MainButton* button : buttons)
	{
		button->handleInput(e, window);
	}


	if (configButton->isClicked(true))
	{
		configButtonClicked = true;
	}
	if (exitButton->isClicked(true))
	{
		exitButtonClicked = true;
	}
	if (QLButton->isClicked(true))
	{
		quickLoadButtonClicked = true;
	}
	if (loadButton->isClicked(true))
	{
		loadButtonClicked = true;
	}
	if (QSButton->isClicked(true))
	{
		quickSaveButtonClicked = true;
	}
	if (saveButton->isClicked(true))
	{
		saveButtonClicked = true;
	}
	if (autoButton->isClicked(true))
	{
		autoButtonClicked = true;
	}
	if (skipButton->isClicked(true))
	{
		skipButtonClicked = true;
	}
}

void DrawMainButton::render(sf::RenderWindow & window)
{
	for (MainButton* button : buttons)
	{
		window.draw(*button);
	}
}

void DrawMainButton::update(float delta_t)
{
	configButton->update(delta_t);
	exitButton->update(delta_t);
	QLButton->update(delta_t);
	loadButton->update(delta_t);
	QSButton->update(delta_t);
	saveButton->update(delta_t);
	autoButton->update(delta_t);
	skipButton->update(delta_t);


	//for (MainButton* button : buttons)
	//{
	//	if (button->isClicked(true))
	//	{
	//		somethingClicked = true;
	//		if (button == configButton)
	//		{
	//			configButtonClicked = true;
	//		}
	//		else if (button == exitButton)
	//		{
	//			exitButtonClicked = true;
	//		}
	//		else if (button == QLButton)
	//		{
	//			quickLoadButtonClicked = true;
	//		}
	//		else if (button == LoadButton)
	//		{
	//			loadButtonClicked = true;
	//		}
	//		else if (button == QSButton)
	//		{
	//			quickSaveButtonClicked = true;
	//		}
	//		else if (button == saveButton)
	//		{
	//			saveButtonClicked = true;
	//		}
	//		else if (button == autoButton)
	//		{
	//			autoButtonClicked = true;
	//		}
	//		else if (button == skipButton)
	//		{
	//			skipButtonClicked = true;
	//		}
	//		// FIX THIS
	//	}
	//}

}

void DrawMainButton::cleanup()
{

}

DrawMainButton::DrawMainButton()
{
	init();
}

DrawMainButton::~DrawMainButton()
{

}
