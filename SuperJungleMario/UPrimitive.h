#pragma once
#include "URenderer.h"

class UPrimitive
{
	public:
	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
	virtual bool CollisionCheck(UPrimitive* other);

	virtual ~UPrimitive() {};

	static float inline scaleMod = 0.1f;

	FVector Location;
	FVector Velocity;
	float width;
	float height;
	float Radius;
	bool bisHold;

	// 텍스쳐 포인터 저장
	ID3D11ShaderResourceView* TextureSRVPtr[2] = { nullptr, nullptr };

};