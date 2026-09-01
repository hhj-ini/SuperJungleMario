#pragma once
#include "UBox.h"
#include <unordered_set>

class UBrick :
    public UBox
{
public:
    virtual bool CollisionCheck(UPrimitive* other) override;
    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;

    // Box 전용 함수
    virtual void SetBoxState(EBoxType InType);

    // 머리 위에 있는 적들의 리스트
    std::unordered_set<UPrimitive*> EnemyList;
};

