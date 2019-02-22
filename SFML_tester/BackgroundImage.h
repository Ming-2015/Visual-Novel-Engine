#pragma once
#include "ItemImage.h"

class BackgroundImage : public ItemImage
{
public:
	BackgroundImage(string name, string expression, float xPos, float yPos) :
		ItemImage(name, expression, xPos, yPos) {}

	BackgroundImage(ifstream& file) :
		ItemImage(file) {}

	~BackgroundImage() {}
};
