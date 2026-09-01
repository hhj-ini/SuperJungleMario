#pragma once
#include "UPrimitive.h"


class UBall : public UPrimitive
{
public:
	static int inline TotalNumBalls = 0;

	static float inline scaleMod = 0.5f;

	float Radius = 1.0f;
	float Mass = 1.0f;

	float Index;

	float DampingAmount = 0.95f;
	float GravityAmount = 0.058f;

	float DeltaGravityAmount = 0.098f * 60;

	bool bIsHold = false;

public:
	UBall();

	virtual ~UBall();

	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);

	virtual bool CollisionCheck(UPrimitive* other);

	virtual void Move();

	virtual void UpdateVelocity(bool bGravity);
};

