#include "UFlower.h"
#include "ResourceManager.h"
UFlower::UFlower(float x, float y, float w, float h) : UBall(x, y, w, h)
{
	ObjectType = EObjectType::FLOWER;
	Location.x = x;
	Location.y = y;
	width = w * scaleMod;
	height = h * scaleMod;
	bisMove = false;
	InitHeight= y;

}

void UFlower::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) {
	if (fState == FlowerState::ENABLE || fState == FlowerState::ANIMATING)
	{	// 꽃이 애니메이션 중이거나, 활성화 상태일 때만 렌더링

		/*renderer.UpdateConstantBuffer(Location, Radius);
		renderer.RenderPrimitive(pBuffer, num);*/

		// 렌더하기 전에 텍스쳐 바인딩
		if (!TextureSRVPtr[0])
		{
			TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Flower.png", &renderer);
		}
		renderer.PrepareShaderResource(TextureSRVPtr[0]);

		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

		renderer.RenderPrimitive(pBuffer, num);
	}
}
 
bool UFlower::CollisionCheck(UPrimitive* other) {
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
		case EObjectType::PLAYER: 
			fState = FlowerState::DESTROYED; // 플레이어와 충돌 시 꽃을 DESTROYED 상태로 변경

			// 꽃을 먹어도 점수가 1000점 올라감

			break;

		default:
			break;
		}
	}

	return false;
}


void UFlower::SetAnimation() {
	Velocity.y = 0.01f; // 꽃이 위로 올라가는 속도 설정
	fState = FlowerState::ANIMATING; // 꽃 상태를 ANIMATING으로 변경
}

void UFlower::Move() {
	if (fState == FlowerState::ANIMATING) {
		Location.y += 0.01f; // 꽃이 위로 올라감
		if (Location.y >= UFlower::InitHeight+height) {
			Velocity.y = 0.0f;
			fState = FlowerState::ENABLE; // 꽃이 다 올라오면 ENABLE 상태로 변경
		}
	}
}



