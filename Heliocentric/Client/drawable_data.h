#pragma once
#include "texture.h"

/**
A class which holds important data about how something should be drawn
*/
class DrawableData {
public:
	DrawableData();
	DrawableData(Texture texture);

	Texture texture;
};