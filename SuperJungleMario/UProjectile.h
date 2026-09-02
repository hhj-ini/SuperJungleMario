#pragma once
#include "UBall.h"

class UPlayer;

class UProjectile :
    public UBall
{
  
public:
    enum class EProjectileState
    {   // 대기, 공격, 충돌, 
        WAITING, /* 대기 */
        ROLLING,
        HIT
    };

	UProjectile();

    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    virtual bool CollisionCheck(UPrimitive* other) override;
    virtual void Move() override;

    void SetState(EProjectileState InState);
    void SetOwner(UPrimitive* InOwner);

	bool ActivateProjectile(FVector PlayerLocation, bool bFacingLeft, float playerWidth);
    void DeactivateProjectile();

    virtual void UpdateAnimation(float deltaTime);
    virtual void UpdateVelocity(bool bGravity);

private:
    EProjectileState CurrState = EProjectileState::WAITING;

    UPlayer* Owner;
	bool bActive = false;
	int LifeTime = 0;
	int MaxLifeTime = 3;

    int CurrentFrame = 0;
    float AnimationTimer = 0.0f;
    const float FrameInterval = 0.15f;
};

