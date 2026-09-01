#pragma once
#include "UBall.h"
#include "UPlayer.h"

class UEnemy : public UBall
{
public:
    enum class EnemyState
    {
        ALIVE,
        DEAD
    };

    const FVector& GetPosition() const { return Location; }
    

public:
    UEnemy();

    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
    virtual bool CollisionCheck(UPrimitive* other);
    bool IsEnemyDead() const { return eState == EnemyState::DEAD; }

    virtual void Move() override;

    void SetState(EnemyState InState);
    void OnDeath(UPlayer* player);

    virtual void UpdateAnimation(float deltaTime);

private:
    EnemyState eState = EnemyState::ALIVE;

    int CurrentFrame = 0;
    float AnimationTimer = 0.0f;
    const float FrameInterval = 0.15f;
};
