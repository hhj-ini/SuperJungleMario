#pragma once
#include "UBall.h" 
#include "ResourceManager.h"

class UFlower : public UBall
{
public:
	UFlower(float x, float y, float w, float h);
	~UFlower();
	enum class FlowerState
	{
		WAITING,
		ANIMATING,
		ENABLE
	};
	bool bIsAlive = true;

	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
	virtual bool CollisionCheck(UPrimitive* other) override;
	virtual void Move() override;

	void SetEnable();

	void SetAnimation();

	void SetState(FlowerState InState);


private:
};