#include "UMushroom.h"
#include "ResourceManager.h"

UMushroom::UMushroom(float x, float y, float w, float h) : UBall(x, y, w, h)
{

	ObjectType = EObjectType::MUSHROOM;
	Location.x = x;
	Location.y = y;
	width = w * scaleMod;
	height = h * scaleMod;
	bisMove = false;
	StartAnimLocationY = y;

} 

void UMushroom::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	if (mrState == MushroomState::ENABLE || mrState == MushroomState::ANIMATING)
	{	// 버섯이 애니메이션 중이거나, 활성화 상태일 때만 렌더링

		/*renderer.UpdateConstantBuffer(Location, Radius);
		renderer.RenderPrimitive(pBuffer, num);*/

		// 렌더하기 전에 텍스쳐 바인딩
		if (!TextureSRVPtr[0])
		{
			TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Mushroom.png", &renderer);
		}
		renderer.PrepareShaderResource(TextureSRVPtr[0]);

		//DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

		renderer.RenderPrimitive(pBuffer, num);
	}
}

bool UMushroom::CollisionCheck(UPrimitive* other)
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

		if (mrState != MushroomState::ENABLE) {
			// 활성화 상태 이외에는 충돌 처리하지 않음
			return false;
		}

		switch (other->ObjectType)
		{
		case (EObjectType::BOX): // 박스와 충돌 시 처리
			if (overlapX > overlapY) { //y축방향으로 충돌시 y속도 0으로 처리
				Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
				Velocity.x = 0.005f;
				Velocity.y = 0;
			}
			else { // x축방향으로 충돌시 x속도 0으로 처리
				Location.x = other->Location.x + (other->width / 2.0f) + (width / 2.0f);
				Velocity.x *= -1.0f;
			}
			break;

		case EObjectType::PLAYER:
			mrState = MushroomState::DESTROYED;
			// 버섯을 먹으면 점수가 1000점 올라감
			UGameLogic::GetInstance().addScore(1000, Location.x, Location.y);
			break;
		case EObjectType::ENEMY:
			// 적과 충돌 시 처리
			break;
		}
	}

	return false;

}

void UMushroom::Move()
{
	if (mrState == MushroomState::WAITING)
	{	// wating 상태에서는 움직이지 않음. early return
		return;
	}
	

	if (mrState == MushroomState::ANIMATING) {
		Location.y += 0.01f; // 꽃이 위로 올라감
		if (Location.y >= UMushroom::StartAnimLocationY + height) {
			Velocity.y = 0.0f;
			mrState = MushroomState::ENABLE; // 꽃이 다 올라오면 ENABLE 상태로 변경
			return;
		}
	}
	this->UBall::Move();
}

void UMushroom::SetState(MushroomState InState)
{	
	switch (InState)
	{
	case MushroomState::ANIMATING:
		mrState = MushroomState::ANIMATING;
		bisMove = true;
		// 버섯 들어있던 물음표 박스 충돌 시 SetState(MushroomState::ANIMATING); 호출
		Velocity.y = 0.001f;
		//StartAnimLocationY = Location.y;
		break;

	case MushroomState::ENABLE:
		// 애니메이션 끝나면 
		// SetState(MushroomState::ENABLE); 호출
		mrState = MushroomState::ENABLE;
		Velocity.x = +0.01f;
		Velocity.y = 0.0f;
		break;

	case MushroomState::WAITING:
		bisMove = false;
		break;
	
	case MushroomState::DESTROYED:
		mrState = MushroomState::DESTROYED;
		break;
	}
}
void UMushroom::SetAnimation() {
	SetState(MushroomState::ANIMATING);
}

void UMushroom::UpdateVelocity(bool bGravity)
{
	if (mrState == MushroomState::ANIMATING)
	{
		// 애니메이션 중에는 중력 적용하지 않음
		return;
	}
	UBall::UpdateVelocity(bGravity);
}
