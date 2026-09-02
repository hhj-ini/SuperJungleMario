#pragma once
#include "UBrick.h"
class UQuestionBox :
    public UBrick
{
public:
	enum class EItemType
	{
		MUSHROOM,	// 버섯 아이템
        FLOWER,
        COIN
	};
    UQuestionBox();
    UQuestionBox(float x, float y, float w, float h, EItemType itemType );

    //virtual bool CollisionCheck(UPrimitive* other) override;
    virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    virtual void OnHitFromBelow() override;
    virtual void SetSoundResource(USoundManager* soundManager) override;

	EItemType myItemType;
	UPrimitive* ItemPtr = nullptr;	// 박스에서 나오는 아이템 포인터
};

