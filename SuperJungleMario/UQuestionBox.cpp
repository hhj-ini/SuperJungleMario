#include "UQuestionBox.h"
#include "UMushroom.h"
#include "ResourceManager.h"

UQuestionBox::UQuestionBox(float x, float y, float w, float h, EItemType itemType)
{
	Location.x = x;
	Location.y = y;
	width = w * scaleMod;
	height = h * scaleMod;

	ObjectType = EObjectType::BOX;
	BoxType = EBoxType::QUESTION;
	myItemType = itemType;
}

//UQuestionBox::UQuestionBox(float x, float y, float w, float h)
//{
//	ObjectType = EObjectType::BOX;
//	ItemPtr = new UMushroom;	// flower 위해서 수정 필요
//	BoxType = EBoxType::QUESTION;
//
//	Location.x = x;
//	Location.y = y;
//	width = w * scaleMod;
//	height = h * scaleMod;
//}

void UQuestionBox::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	if (!TextureSRVPtr[0])
	{
		TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\QMBlock.png", &renderer);
	}
	renderer.PrepareShaderResource(TextureSRVPtr[0]);

	DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);

	renderer.UpdateConstantBuffer(world, renderer.ViewMatrix, AnimOffset);

	renderer.RenderPrimitive(pBuffer, num);
}
