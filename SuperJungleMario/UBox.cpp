#include "UBox.h"

UBox::UBox()
{
	Location.x = ((float)(rand() % 180 - 90)) * 0.01f;
	Location.y = ((float)(rand() % 180 - 90)) * 0.01f;


	Radius = 0.0001f;
	TotalNumBox++;

	width = Radius * scaleMod;
	height = Radius * scaleMod;
	bisHold = false;
}

UBox::~UBox()
{
	--TotalNumBox;
}


void UBox::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	renderer.UpdateConstantBuffer(Location, Radius);
	renderer.RenderPrimitive(pBuffer, num);
}