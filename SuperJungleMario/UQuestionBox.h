#pragma once
#include "UBrick.h"
class UQuestionBox :
    public UBrick
{
public:
    UQuestionBox();

    //virtual bool CollisionCheck(UPrimitive* other) override;
    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;


    UPrimitive* ItemPtr;   // 물음표 박스 안에 숨겨져있는 아이템 포인터
};

