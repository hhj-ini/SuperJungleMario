#include "UBox.h"

UBox::UBox(float x, float y, float w, float h)
{
	Location.x = x;
	Location.y = y;
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
