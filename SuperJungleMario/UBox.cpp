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
	ObjectType = EObjectType::BOX;

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


bool UBox::CollisionCheck(UPrimitive* other) 
{
	// 기본 충돌 체크 로직 구현

	// 가로가 겹치는지 확인
	float sumHalfWidth = (width / 2.0f) + (other->width / 2.0f);
	float xdistance = std::fabs(Location.x - other->Location.x);
	float overlapX = sumHalfWidth - xdistance;

	// 세로가 겹치는지 확인
	float sumHalfHeight = (height / 2.0f) + (other->height / 2.0f);
	float ydistance = std::fabs(Location.y - other->Location.y);
	float overlapY = sumHalfHeight - ydistance;


	// 충돌
	if (overlapX > 0 && overlapY > 0) {

		switch (other->ObjectType)
		{
		default:
			break;

		}

		return true;
	}

		return false;
}

UBox::~UBox()
{
	--TotalNumBox;
}
