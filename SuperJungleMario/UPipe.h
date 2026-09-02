#pragma once
#include "UPrimitive.h"
#include "UBox.h"

class UPipe : public UBox
{
public:

	//int Index;
	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;

	virtual bool CollisionCheck(UPrimitive* other) override ;

	
public:
	UPipe(float locationX, float locationY, float width, float height); //위치x,y,  가로,세로
	virtual ~UPipe() {}

};
