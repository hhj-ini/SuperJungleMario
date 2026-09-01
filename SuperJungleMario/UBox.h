#pragma once
#pragma once
#include "UPrimitive.h"


class UBox : public UPrimitive
{
public:
	static int inline TotalNumBox = 0;

	//int Index;
	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;

public:
	UBox();
	UBox(float locationX, float locationY, float width, float height); //위치x,y,  가로,세로

	virtual ~UBox();


	//virtual bool CollisionCheck(UPrimitive* other);

};

