#pragma once
#include "UBall.h"
#include "UPlayer.h"
#include "UGameLogic.h"

class UEnemy : public UBall
{
public:
    enum class EnemyState
    {
        ALIVE,
        DEAD
    };

    const FVector& GetPosition() const { return Location; }
    UPlayer* Player = nullptr;

public:
    UEnemy(float locationX, float locationY, float width, float height);

    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
    virtual bool CollisionCheck(UPrimitive* other);
    bool IsEnemyDead() const { return eState == EnemyState::DEAD; }

    virtual void Move() override;

    void SetPlayer(UPlayer* InPlayer);
    void SetState(EnemyState InState);
    void OnDeath(UPlayer* player);
    float GetHeight() const { return height; }

    virtual void UpdateAnimation(float deltaTime);
    virtual void SetSoundResource(USoundManager* soundManager) override;
    virtual void UpdateVelocity(bool bGravity) override;

private:
    EnemyState eState = EnemyState::ALIVE;

    int CurrentFrame = 0;
    float AnimationTimer = 0.0f;
    const float FrameInterval = 0.15f;
};
