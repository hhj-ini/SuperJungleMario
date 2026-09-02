#pragma once
#include "UPrimitive.h"
#include "ResourceManager.h"

class UBackground : public UPrimitive
{
public:
	UBackground(float locationX, float loactionY, float width, float height);
	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
	virtual void CollisionCheck();
};