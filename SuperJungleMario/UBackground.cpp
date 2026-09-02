#include "UBackground.h"

UBackground::UBackground(float locationX, float locationY, float width, float height)
{
	UBackground::Location.x = locationX;
	UBackground::Location.y = locationY;
	UBackground::width = width;
	UBackground::height = height;
}

void UBackground::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	if (!TextureSRVPtr[0])
	{
		TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\world.png", &renderer);
	}
	renderer.PrepareShaderResource(TextureSRVPtr[0]);
	
	DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
	renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);
	renderer.RenderPrimitive(pBuffer, num);
}

void UBackground::CollisionCheck()
{
	//empty
}