#include "UFlowerBox.h"
#include "UFlower.h"
#include  "ResourceManager.h"

UFlowerBox::UFlowerBox() {
	ObjectType = EObjectType::BOX;
	//ItemPtr = new UFlower;
	BoxType = EBoxType::QUESTION;

	// 테스트용 좌표 설정
}

void UFlowerBox::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
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
