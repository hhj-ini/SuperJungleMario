#pragma once
#include "UBox.h"
#include <unordered_set>

class UBrick :
    public UBox
{
public:
    enum class EAnimState
    {
        STOP,
        UP,
        DOWN
    };
    UBrick();
    UBrick(float x, float y, float w, float h);

    virtual bool CollisionCheck(UPrimitive* other) override;
    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    virtual void Tick() override;
    virtual void OnHitFromBelow();
    // Box 전용 함수
    virtual void SetAnimState(EAnimState InType);


    // 머리 위에 있는 적들의 리스트
    std::unordered_set<UPrimitive*> EnemyList;

    FVector AnimOffset;
    EAnimState AnimState = EAnimState::STOP;
    };

