#pragma once
#include "UBall.h"
#include "UGameLogic.h"

/*
Flower 작동구조
1. 마리오가 박스를 때림
2. 박스가 꽃 생성 -> FlowFlower() 호출
3. 꽃 떠오름 
4. 꽃 다 떠오름
5. 꽃 활성화
6. 마리오가 꽃 먹음
7. 꽃 사라짐
*/
class UFlower :
    public UBall
{
public:

    enum class FlowerState
    {
        WAITING, // 블럭속에서 기다리고있음
		ANIMATING, // 블럭에서 올라오는 애니메이션 중
		ENABLE, // 활성화 상태, 플레이어가 먹을 수 있음
		DESTROYED // 플레이어가 먹어서 사라짐
    };

	UFlower(float x, float y, float w, float h)
	{
		ObjectType = EObjectType::FLOWER;
		Location.x = x;
		Location.y = y;
		width = w * scaleMod;
		height = h * scaleMod;
		bisMove = false;
        InitHeight= y;
	}
    
    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    virtual bool CollisionCheck(UPrimitive* other)override;

    virtual void Move() override;
    bool IsFlowerDestroyed() const { return fState == FlowerState::DESTROYED; }

    void SetEnable();

    void SetAnimation();

    void FlowFlower();
    // void UpdateVelocity(bool bGravity) override;


private:
    FlowerState fState = FlowerState::WAITING;
    float InitHeight;
};

