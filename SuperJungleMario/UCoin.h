#pragma once
#include "UBall.h"
#include "UGameLogic.h"

class UCoin :
    public UBall
{
public:

    enum class CoinState
    {
        WAITING, // 블럭속에서 기다리고있음
        ANIMATING, // 블럭에서 올라오는 애니메이션 중
        DESTROYED // 플레이어가 먹어서 사라짐
    };



    UCoin(float x, float y, float w, float h);


    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    virtual bool CollisionCheck(UPrimitive* other)override;

    virtual void Move() override;
    bool IsFlowerDestroyed() const { return fState == CoinState::DESTROYED; }

    void SetEnable();

    CoinState GetState() { return fState; }


    void SetAnimation();

    void FlowCoin();
    // void UpdateVelocity(bool bGravity) override;


private:
    CoinState fState = CoinState::WAITING;
    float InitHeight;
};

