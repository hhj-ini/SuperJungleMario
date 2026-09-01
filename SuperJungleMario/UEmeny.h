#pragma once
#include "UBall.h"
#include "UPlayer.h"

class UEmeny : public UBall
{
public:
    enum class EmenyState
    {
        ALIVE,
        DEAD
    };

    const FVector& GetPosition() const { return Location; }
    

public:
    UEmeny();

    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
    virtual bool CollisionCheck(UPrimitive* other);
    bool IsEnemyDead() const { return eState == EmenyState::DEAD; }

    virtual void Move() override;

    void SetState(EmenyState InState);
    void OnCollisionWithPlayer(UPlayer* player);


private:
    EmenyState eState = EmenyState::ALIVE;
};


