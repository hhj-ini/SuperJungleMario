#pragma once
#include "URenderer.h"

class UPrimitive
{
	public:
	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
	virtual bool CollisionCheck(UPrimitive* other);
	static float inline scaleMod = 0.1f;
	virtual ~UPrimitive() {};

	FVector Location;
	FVector Velocity;
	float width;
	float height;
	float Radius;
	bool bisMove=true;
};