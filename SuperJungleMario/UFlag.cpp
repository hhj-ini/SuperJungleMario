#include "UFlag.h"
#include "ResourceManager.h"

UFlag::UFlag(float locationX, float locationY, float inWidth, float inHeight)
{
	Location.x = locationX;
	Location.y = locationY;
	width = inWidth * scaleMod;
	height = inHeight * scaleMod;
}

void UFlag::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	if (!TextureSRVPtr[0])
	{
		TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Flag.png", &renderer);
	}
	renderer.PrepareShaderResource(TextureSRVPtr[0]);

	DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);

	renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

	renderer.RenderPrimitive(pBuffer, num);
}

bool UFlag::CollisionCheck(UPrimitive * other)
{
	if (EObjectType::PLAYER == other->ObjectType)
	//{
	//	UGameLogic::GetInstance().addScore(3000);
	//	UGameLogic::GetInstance().setEnding();
		// 스테이지 클리어
	//}
    return false;
}
