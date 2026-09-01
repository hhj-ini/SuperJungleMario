#pragma once
#include "UBall.h"


class UMushroom :
    public UBall
{
public:
    UMushroom();
    enum class MushroomState
    {
        WAITING,
        ANIMATING,
        ENABLE
    };

    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
    virtual bool CollisionCheck(UPrimitive* other);

    virtual void Move() override;

    void SetEnable();

    void SetAnimation();

    void SetState(MushroomState InState);

private:
    MushroomState mrState = MushroomState::WAITING;
    float StartAnimLocationY = 0.0f;
};

