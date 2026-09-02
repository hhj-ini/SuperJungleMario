#pragma once
#include "UPrimitive.h"
class UFlag :
    public UPrimitive
{
public:

    UFlag(float locationX, float locationY, float width, float height); //위치x,y,  가로,세로

    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    virtual bool CollisionCheck(UPrimitive* other) override;


};

