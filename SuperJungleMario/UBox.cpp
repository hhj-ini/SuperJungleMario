#include "UBox.h"

UBox::UBox(float x, float y, float w, float h)
{
	Location.x = x*scaleMod;
	Location.y = y * scaleMod;
	width = w * scaleMod;
	height = h * scaleMod;

	Radius = 0.0001f;
	++TotalNumBox;

	bisHold = false;
}

UBox::~UBox()
{
	--TotalNumBox;
}
