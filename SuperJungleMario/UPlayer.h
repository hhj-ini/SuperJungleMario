#pragma once
#include "UBall.h"
#include "UGameLogic.h"

class UPlayer : public UBall
{
public:
    bool bIsGrounded = true;
    int Hp = 1;
    bool bFacingLeft = false;
	bool bBigMario = false;
	bool bFireMario = false;
	bool bAttacking = false;
    bool bShotFireRequest = false;

    const FVector& GetPosition() const { return Location; }
	const FVector& GetPreviousPosition() const { return PreviousLocation; }
    float GetHeight() const { return height; }
	float GetWidth() const { return width; }
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

    virtual void Tick(float deltaTime) override;
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
    int GetBaseFrame() const;
	void FireMario();
	void RequestFire();
    bool ShotFireRequest();
    void Reset();

private:
    PlayerState pState = PlayerState::ALIVE;

    FVector PreviousLocation;

    int CurrentFrame = 0;
    float AnimationTimer = 0.0f;
    const float FrameInterval = 0.15f;

    float DamageTimer = 0.0f;
    const float DamageInvincibleTime = 1.0f;

    float FireTimer = 0.0f;
    const float FireInterval = 0.6f;
};