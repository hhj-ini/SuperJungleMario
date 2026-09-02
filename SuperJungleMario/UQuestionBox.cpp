#include "UQuestionBox.h"
#include "UMushroom.h"
#include "ResourceManager.h"

UQuestionBox::UQuestionBox()
{
	ObjectType = EObjectType::BOX;
	ItemPtr = new UMushroom;	
	BoxType = EBoxType::QUESTION;

	// 테스트용 좌표 설정
	Location.x = 1.0f * scaleMod;
}

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
