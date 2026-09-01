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
        ENABLE,
		DESTROYED
    };

    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    virtual bool CollisionCheck(UPrimitive* other);

    virtual void Move() override;
    bool IsMushroomDestroyed() const { return mrState == MushroomState::DESTROYED; }

    void SetEnable();

    void SetAnimation();

    void SetState(MushroomState InState);
    // void UpdateVelocity(bool bGravity) override;

private:
    MushroomState mrState = MushroomState::WAITING;
    float StartAnimLocationY = 0.0f;
};

