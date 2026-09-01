#include "UEmeny.h"
#include "UPlayer.h"

UEmeny::UEmeny() : UBall()
{
	Velocity.x = 0.0f;
	Velocity.y = 0.0f;

	Location.x = 0.0f;
	Location.y = 0.0f;
	// 위치 설정 필요

	Radius = 1.0f;
}

void UEmeny::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	if (eState == EmenyState::ALIVE)
	{
		DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

		renderer.RenderPrimitive(pBuffer, num);
	}
}

bool UEmeny::CollisionCheck(UPrimitive* other)
{
	if (this == other)
	{
		return false;
	}

	return false;
}

void UEmeny::Move()
{
	if (eState == EmenyState::ALIVE)
	{
		this->UBall::Move();
	}
}

void UEmeny::SetState(UEmeny::EmenyState InState)
{
	switch (InState)
	{
	case EmenyState::ALIVE:
		eState = EmenyState::ALIVE;
		break;

	case EmenyState::DEAD:
		eState = EmenyState::DEAD;
		break;
	}
}

void UEmeny::OnCollisionWithPlayer(UPlayer* player)
{
	if (eState == EmenyState::ALIVE)
	{
		if (player->GetPosition().y > this->GetPosition().y + 0.5f)  // 플레이어가 적을 밟았을 때
		{
			SetState(EmenyState::DEAD);
			player->SetVelocityY(0.2f); // 플레이어가 튕기도록 설정
		}
		else
		{
			// 플레이어가 적과 충돌했을 때
			player->TakeDamage(1);
		}
	}

}