#include "UMushroom.h"
#include "ResourceManager.h"

UMushroom::UMushroom()
	: UBall()
{
	Velocity.x = 0.0f;
	Velocity.y = 0.0f;
	// y 축 초기속도 갖지 않도록 함.

	Location.x = 0.0f;
	Location.y = 0.0f;
	// 물음표 위치로 애니메이션 이전에 위치 설정 필요함

	Radius = 0.75f;
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

		DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

		renderer.RenderPrimitive(pBuffer, num);
	}
}

bool UMushroom::CollisionCheck(UPrimitive* other)
{
	if (this == other)
	{
		return false;
	}
	
	return false;
}

void UMushroom::Move()
{
	if (mrState == MushroomState::WAITING)
	{	// wating 상태에서는 움직이지 않음. early return
		return;
	}
	this->UBall::Move();

	if (mrState == MushroomState::ANIMATING && Location.y - StartAnimLocationY > 0.1f)
	{
		// 위로 올라가는 애니메이션 멈추기
		SetState(MushroomState::ENABLE);
	}
}

void UMushroom::UpdateVelocity(bool bGravity)
{
	return; // 버섯에서는 속도처리 하지 않도록 함
}

void UMushroom::SetState(MushroomState InState)
{	
	switch (InState)
	{
	case MushroomState::ANIMATING:
		// 버섯 들어있던 물음표 박스 충돌 시 SetState(MushroomState::ANIMATING); 호출
		mrState = MushroomState::ANIMATING;
		Velocity.y = 0.01f;
		StartAnimLocationY = Location.y;
		break;

	case MushroomState::ENABLE:
		// 애니메이션 끝나면 
		// SetState(MushroomState::ENABLE); 호출
		mrState = MushroomState::ENABLE;
		Velocity.x = 0.01f;
		Velocity.y = 0.0f;

		UpdateVelocity(true);

		break;

	case MushroomState::WAITING:

		break;
	
	}
}
