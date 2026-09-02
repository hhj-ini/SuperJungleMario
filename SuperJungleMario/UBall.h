#pragma once
#include "UPrimitive.h"


class UBall : public UPrimitive
{
public:
	static int inline TotalNumBalls = 0;

	float Mass = 1.0f;

	float Index;

	float DampingAmount = 0.95f;
	float GravityAmount = 0.003f;

	float DeltaGravityAmount = 0.098f * 60;


public:
	UBall();
	UBall(float x, float y, float w, float h);
	virtual ~UBall();

	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
	virtual void SetSoundResource(USoundManager* soundManager) override {};
	//virtual bool CollisionCheck(UPrimitive* other);

	virtual void Move();

	virtual void UpdateVelocity(bool bGravity);

	virtual void UpdateAnimation(float deltaTime);
};

