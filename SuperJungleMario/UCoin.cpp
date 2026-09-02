#include "UCoin.h"
#include "ResourceManager.h"
UCoin::UCoin(float x, float y, float w, float h) : UBall(x, y, w, h)
{
	ObjectType = EObjectType::COIN;
	Location.x = x;
	Location.y = y;
	width = w * scaleMod;
	height = h * scaleMod;
	bisMove = false;
	InitHeight = y;
}

void UCoin::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) {
	if (fState == CoinState::ANIMATING)
	{	// 꽃이 애니메이션 중이거나, 활성화 상태일 때만 렌더링

		/*renderer.UpdateConstantBuffer(Location, Radius);
		renderer.RenderPrimitive(pBuffer, num);*/

		// 렌더하기 전에 텍스쳐 바인딩
		if (!TextureSRVPtr[0])
		{
			TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Coin.png", &renderer);
		}
		renderer.PrepareShaderResource(TextureSRVPtr[0]);

		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

		renderer.RenderPrimitive(pBuffer, num);
	}
}

bool UCoin::CollisionCheck(UPrimitive* other) {
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
			fState = CoinState::DESTROYED; // 플레이어와 충돌 시 꽃을 DESTROYED 상태로 변경
			// 꽃을 먹어도 점수가 1000점 올라감
		

			break;

		default:
			break;
		}
	}

	return false;
}

void UCoin::SetAnimation() {
	UGameLogic::GetInstance().addScore(200, Location.x, Location.y);
	UGameLogic::GetInstance().addOneCoin();
	Velocity.y = 0.08f; // 꽃이 위로 올라가는 속도 설정
	fState = CoinState::ANIMATING; // 꽃 상태를 ANIMATING으로 변경
}

void UCoin::Move() {
	if (fState == CoinState::ANIMATING) {
		Location.y += 0.04f; // 코인이 위로 올라감
		if (Location.y >= UCoin::InitHeight + height * 1.5f) {
			Velocity.y = 0.0f;
			fState = CoinState::DESTROYED; // 꽃이 다 올라오면 ENABLE 상태로 변경
		}
	}
}
