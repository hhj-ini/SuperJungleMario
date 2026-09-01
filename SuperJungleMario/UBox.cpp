#include "UBox.h"
#include "ResourceManager.h"

UBox::UBox(float x, float y, float w, float h)
{
	Location.x = x;
	Location.y = y;
	width = w * scaleMod;
	height = h * scaleMod;

	Radius = 0.0001f;
	++TotalNumBox;
	bisMove = false;

}

void UBox::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
		if (!TextureSRVPtr[0])
		{
			TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Ground.png", &renderer);
		}
		renderer.PrepareShaderResource(TextureSRVPtr[0]);

		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f)*DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);
		renderer.RenderPrimitive(pBuffer, num);
}

UBox::~UBox()
{
	--TotalNumBox;
}
