#include "Dropbox.h"

Dropbox::~Dropbox()
{
	if (mainButton) delete mainButton;
	for (int i = 0; i < dropButtons.size(); i++)
	{
		if (dropButtons[i]) delete dropButtons[i];
	}
}

void Dropbox::setSelected(int selectedId)
{
	this->itemSelected = selectedId;
	if (itemSelected < 0 || itemSelected >= numItems)
	{
		mainButton->setText("");
		return;
	}
	mainButton->setFont(dropButtons[selectedId]->getFont());
	mainButton->setText(dropButtons[selectedId]->getText());
}

int Dropbox::getSelected() const
{
	return itemSelected;
}

std::string Dropbox::getSelectedText() const
{
	return dropButtons[itemSelected]->getText();
}

std::string Dropbox::getSelectedFont() const
{
	return dropButtons[itemSelected]->getFont();
}

std::string Dropbox::getTextById(int i) const
{
	if (i >= numItems) return "";

	return dropButtons[i]->getText();
}

std::string Dropbox::getFontById(int i) const
{
	if (i >= numItems) return "";

	return dropButtons[i]->getFont();
}

void Dropbox::setPosition(int xPos, int yPos)
{
	int xOffset = xPos - this->xPos;
	int yOffset = yPos - this->yPos;
	this->xPos = xPos;
	this->yPos = yPos;

	mainButton->move(xOffset, yOffset);
	for (int i = 0; i < numItems; i++)
	{
		dropButtons[i]->move(xOffset, yOffset);
	}
}

void Dropbox::move(int x, int y)
{
	this->xPos += x;
	this->yPos += y;

	mainButton->move(x, y);
	for (int i = 0; i < numItems; i++)
	{
		dropButtons[i]->move(x, y);
	}
}

void Dropbox::setOffset(int xOffset, int yOffset)
{
	mainButton->setOffset(xOffset, yOffset);
	for (int i = 0; i < numItems; i++)
	{
		dropButtons[i]->setOffset(xOffset, yOffset);
	}
}

void Dropbox::setText(int i, std::string str)
{
	if (i >= numItems) return;

	dropButtons[i]->setText(str);
}

void Dropbox::setFont(int i, std::string fontPath)
{
	if (i >= numItems) return;

	dropButtons[i]->setFont(fontPath);
}

void Dropbox::setAnimationDuration(float duration)
{
	fullAnimationDuration = duration;
}

float Dropbox::getAnimationDuration() const
{
	return fullAnimationDuration;
}

bool Dropbox::onLoad()
{
	mainButton = new DropboxButton(GLOBAL->AssetRoot + "DBMainBox.png",
		GLOBAL->AssetRoot + "DBMainBoxHover.png", xPos, yPos);

	// here's the new drop buttons
	for (int i = 0; i < numItems-1; i++)
	{
		dropButtons.push_back
		(
			new DropboxButton(GLOBAL->AssetRoot + "DBOptions1to3.png",
				GLOBAL->AssetRoot + "DBOptions1to3Hover.png", xPos, yPos)
		);
	}

	// the last one is special
	dropButtons.push_back
	(
		new DropboxButton(GLOBAL->AssetRoot + "DBOption4.png",
			GLOBAL->AssetRoot + "DBOption4Hover.png", xPos, yPos)
	);

	// load the buttons
	mainButton->load();
	for (int i = 0; i < numItems; i++)
	{
		dropButtons[i]->load();
	}

	// the height to move the items
	animationDist.push_back(mainButton->getButtonHeight());
	for (int i = 0; i < numItems - 1; i++)
	{
		animationDist.push_back(dropButtons[i]->getButtonHeight());
	}
	
	// find the total distance
	sumAnimationDist.push_back( animationDist[0] );
	for (int i = 1; i < numItems; i++)
	{
		sumAnimationDist.push_back( sumAnimationDist[i-1] + animationDist[i] );
	}

	return true;
}

void Dropbox::onUpdate(float delta_t)
{
	mainButton->update(delta_t);
	if (drawDroplist)
	{
		for (int i = 0; i < numItems; i++)
		{
			dropButtons[i]->update(delta_t);
		}
	}

	if (animatingDroplist)
	{
		int distOffset;

		if (fullAnimationDuration <= 0)
		{
			distOffset = sumAnimationDist[numItems - 1];
		}
		else
		{
			distOffset = delta_t / fullAnimationDuration * sumAnimationDist[numItems - 1];
		}

		if (shouldShowDroplist)
		{
			// add offset
			currentDist += distOffset;

			// if done animation...
			if (currentDist >= sumAnimationDist[numItems-1])
			{
				currentDist = sumAnimationDist[numItems - 1];
				animatingDroplist = false;
				drawDroplist = true;
			}
		}
		else
		{
			currentDist -= distOffset;
				
			if (currentDist <= 0)
			{
				currentDist = 0;
				animatingDroplist = false;
				drawDroplist = false;
			}
		}


		// animate last few items only
		for (int i = numItems - 1; i >= currentAnimatedItem; i--)
		{
			dropButtons[i]->setPosition(xPos, (dropUp ? yPos - currentDist : yPos + currentDist));
		}

		// reset position of first few items if last item position is reset
		if (currentDist > sumAnimationDist[currentAnimatedItem]
			|| (currentAnimatedItem <= 0 ? currentDist <= 0 : currentDist < sumAnimationDist[currentAnimatedItem - 1]) )
		{
			currentAnimatedItem = findAnimatedItem(sumAnimationDist, currentDist);
			for (int i = 0; i < currentAnimatedItem; i++)
			{
				dropButtons[i]->setPosition(xPos, (dropUp ? yPos - sumAnimationDist[i] : yPos + sumAnimationDist[i]));
			}
		}
		
	}
}

void Dropbox::onDraw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (drawDroplist)
	{
		for (int i = numItems - 1; i >= 0; i--)
		{
			target.draw(*dropButtons[i], states);
		}
	}

	target.draw(*mainButton, states);
}

void Dropbox::onHandleInput(sf::Event & e, sf::RenderWindow & window)
{
	mainButton->handleInput(e, window);
	if (drawDroplist)
	{
		for (int i = 0; i < numItems; i++)
		{
			dropButtons[i]->handleInput(e, window);
		}
	}

	if (mainButton->isClicked(true))
	{
		if (!shouldShowDroplist)
		{
			shouldShowDroplist = true;
			drawDroplist = true;
			animatingDroplist = true;
		}
		else
		{
			shouldShowDroplist = false;
			drawDroplist = true;
			animatingDroplist = true;
		}
	}

	for (int i = 0; i < numItems; i++)
	{
		if (dropButtons[i]->isClicked(true))
		{
			setSelected(i);
			shouldShowDroplist = false;
			animatingDroplist = true;

			break;
		}
	}
}

int Dropbox::findAnimatedItem(const std::vector<int>& sumAnimationDist, const int & currentDist) const
{
	// assuming sorted from small to large
	for (int i = 0; i < sumAnimationDist.size(); i++)
	{
		if (currentDist < sumAnimationDist[i])
		{
			return i;
		}
	}
	return sumAnimationDist.size() - 1;
}
