#pragma once
#include "UBall.h"

class UPlayer : public UBall
{
public:
    bool bIsGrounded = true;
    int Hp = 1;
    bool bFacingLeft = false;
	bool bBigMario = false;

    const FVector& GetPosition() const { return Location; }
    float GetHeight() const { return height; }
    FVector GetVelocity() const { return Velocity; }

    enum class PlayerState
    {
        ALIVE,
        GROWING,  
        SHRINKING,
        DEAD
    };

public:
    UPlayer();
    ~UPlayer() override;

    void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    void UpdateVelocity(bool bGravity) override;
    void Move() override;

    void SetState(PlayerState InState);
    void SetVelocityY(float y) { Velocity.y = y; };
	void TakeDamage();
	void Grow();
	void Shrink();

    virtual void UpdateAnimation(float deltaTime);

    bool IsPlayerDead() const { return pState == PlayerState::DEAD; }
    bool CollisionCheck(UPrimitive* other) override ;
    void Respawn();

private:
    PlayerState pState = PlayerState::ALIVE;

    int CurrentFrame = 0;
    float AnimationTimer = 0.0f;
    const float FrameInterval = 0.15f;

    float DamageTimer = 0.0f;
    const float DamageInvincibleTime = 1.0f;
};