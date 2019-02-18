#pragma once
#include "ItemImage.h"

class CharacterImage : public ItemImage
{
public:
	CharacterImage(string name, string expression, float xPos, float yPos) :
		ItemImage(name, expression, xPos, yPos) {}
	~CharacterImage() {}
};
