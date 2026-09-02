#pragma once
#pragma once
#include "UPrimitive.h"


class UBox : public UPrimitive
{
public:
	enum class EBoxType
	{
		GROUND,		// 땅(안부서짐)
		QUESTION,	// 물음표 박스 (부서지고 이벤트 발생)
		HARD,		// 하드박스 (안부서짐)
		BRICK		// 벽돌박스 (부서지고 이벤트 발생)
		
	};

	static int inline TotalNumBox = 0;

	//int Index;
	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;

	virtual bool CollisionCheck(UPrimitive* other) override ;

	virtual void SetBoxState(EBoxType InType);

public:
	UBox() : UPrimitive()
	{
		ObjectType = EObjectType::BOX;
	}
	UBox(float locationX, float locationY, float width, float height); //위치x,y,  가로,세로
	UBox(float locationX, float locationY, float width, float height, EBoxType bt); //위치x,y,  가로,세로
	virtual ~UBox();


	//virtual bool CollisionCheck(UPrimitive* other) override;
	EBoxType BoxType = EBoxType::GROUND;
};

