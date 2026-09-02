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
    void Move() override;

    void SetState(PlayerState InState);
    void SetVelocityY(float y);
	void TakeDamage(int damage);
	void Grow();
	void Shrink();

    virtual void UpdateAnimation(float deltaTime);

    bool IsPlayerDead() const { return pState == PlayerState::DEAD; }
    bool CollisionCheck(UPrimitive* other) override ;

private:
    PlayerState pState = PlayerState::ALIVE;

    int CurrentFrame = 0;
    float AnimationTimer = 0.0f;
    const float FrameInterval = 0.15f;
};