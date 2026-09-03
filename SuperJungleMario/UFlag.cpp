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
		case EObjectType::PLAYER: // 플레이어와 충돌 시 처리
			Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
			Velocity.x = 0;
			Velocity.y = 0;
			//스테이지 클리어
			UGameLogic::GetInstance().addScore(5000, Location.x, Location.y);
			UGameLogic::GetInstance().setEnding();
			break;
		}
	}

    return false;
}
