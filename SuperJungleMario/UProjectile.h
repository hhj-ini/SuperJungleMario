#pragma once
#include "UBall.h"

class UPlayer;

class UProjectile :
    public UBall
{
    enum class EProjectileState
    {   // 대기, 공격, 충돌, 
        WAITING, /* 대기 */
        ROLLING,
        HIT
    };
public:
    UProjectile();

    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    virtual bool CollisionCheck(UPrimitive* other);

    void SetState(EProjectileState InState);

private:
    EProjectileState CurrState;

    UPlayer* Owner;
};

