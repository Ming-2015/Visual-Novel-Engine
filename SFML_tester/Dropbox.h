#pragma once
#include "Effect.hpp"
#include "DropboxButton.h"

class Dropbox : public Effect{
public:
	Dropbox(int numItems, int xPos, int yPos)
		: numItems(numItems), xPos(xPos), yPos(yPos), Effect("Dropbox")
	{
	}

	~Dropbox();

	// position setter
	void setPosition(int xPos, int yPos);
	void setOffset(int xOffset, int yOffset);

	// dropUp getter/setter
	void setDropUpwards(bool up) { dropUp = up; }
	bool shouldDropUpwards() const { return dropUp; }

	// itemSelected getter/setter
	void setSelected(int selectedId);
	int getSelected() const;

	// text and font getters/setters
	std::string getSelectedText() const;
	std::string getSelectedFont() const;

	std::string getTextById(int i) const;
	std::string getFontById(int i) const;

	void setText(int i, std::string str);
	void setFont(int i, std::string fontPath);

	// animation duration
	void setAnimationDuration(float duration);
	float getAnimationDuration() const;

protected:
	bool onLoad();
	void onUpdate(float delta_t);
	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHandleInput(sf::Event&e, sf::RenderWindow& window);

private:

	int itemSelected = 0;
	int numItems;
	int xPosOffset = 0, yPosOffset = 0;
	float xPos, yPos;

	bool shouldShowDroplist = false;
	bool drawDroplist = false;
	bool animatingDroplist = false;
	bool dropUp = true;

	int currentAnimatedItem = 0;
	int currentDist = 0;
	float fullAnimationDuration = 0.15f;

	DropboxButton* mainButton = nullptr;
	std::vector< DropboxButton* > dropButtons;

	std::vector< int > animationDist;
	std::vector< int > sumAnimationDist;

	int findAnimatedItem(const std::vector<int>& sumAnimationDist, const int& currentDist) const;
};