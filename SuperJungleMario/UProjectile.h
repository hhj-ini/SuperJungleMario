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

private:
    EProjectileState CurrState = EProjectileState::WAITING;

    UPlayer* Owner;
};

