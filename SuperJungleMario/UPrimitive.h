#pragma once
#include "URenderer.h"

class UPrimitive
{
	public:
	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) = 0;
	virtual bool CollisionCheck(UPrimitive* other) = 0;
	virtual void Picking(float xPos, float yPos, bool& bLineRender) = 0;
	virtual void LetGo(FPos HoldPos, FPos CurrPos) = 0;

	virtual ~UPrimitive() {};
};