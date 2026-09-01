#pragma once
#include "UBall.h"

class UPlayer : public UBall
{
public:
    bool bIsGrounded = true;
    int Life = 1;

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

    bool IsPlayerDead() const { return pState == PlayerState::DEAD; }

private:
    PlayerState pState = PlayerState::ALIVE;
};