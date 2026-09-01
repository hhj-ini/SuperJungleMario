#pragma once
#pragma once
#include "UPrimitive.h"


class UBox : public UPrimitive
{
public:
	static int inline TotalNumBox = 0;

	static float inline scaleMod = 0.1f;

	float Radius = 0.01f;

	//int Index;

public:
	UBox();

	virtual ~UBox();

	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);

	//virtual bool CollisionCheck(UPrimitive* other);

};

