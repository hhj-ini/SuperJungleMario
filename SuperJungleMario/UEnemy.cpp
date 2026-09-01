#include "UEnemy.h"
#include "UPlayer.h"
#include "ResourceManager.h"

UEnemy::UEnemy()
{
	Velocity.x = 0.0f;
	Velocity.y = 0.0f;

	Location.x = 0.0f;
	Location.y = 0.0f;
	// 위치 설정 필요

	SetState(EnemyState::ALIVE);
}

void UEnemy::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	if (eState == EnemyState::ALIVE)
	{
		if (!TextureSRVPtr[0])
		{
			TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Goomba1.png", &renderer);
		}

		if (!TextureSRVPtr[1])
		{
			TextureSRVPtr[1] = ResourceManager::GetInstance().GetSRV(L"Resource\\Goomba2.png", &renderer);
		}
		renderer.PrepareShaderResource(TextureSRVPtr[CurrentFrame]);

		DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

		renderer.RenderPrimitive(pBuffer, num);
	}
}

bool UEnemy::CollisionCheck(UPrimitive* other)
{
	if (this == other)
	{
		return false;
	}

	return false;
}

void UEnemy::Move()
{
	if (eState == EnemyState::DEAD)
	{
		return;
	}
	this->UBall::Move();
}

void UEnemy::SetState(UEnemy::EnemyState InState)
{
	switch (InState)
	{
	case EnemyState::ALIVE:
		eState = EnemyState::ALIVE;
		Velocity.x = -0.01f;
		break;

	case EnemyState::DEAD:
		eState = EnemyState::DEAD;
		break;
	}
}

void UEnemy::OnCollisionWithPlayer(UPlayer* player)
{
	if (eState == EnemyState::ALIVE)
	{
		if (player->GetPosition().y > this->GetPosition().y + 0.5f)  // 플레이어가 적을 밟았을 때
		{
			SetState(EnemyState::DEAD);
			player->SetVelocityY(0.2f); // 플레이어가 튕기도록 설정
		}
		else
		{
			// 플레이어가 적과 충돌했을 때
			player->TakeDamage(1);
		}
	}
}

void UEnemy::UpdateAnimation(float deltaTime)
{
	AnimationTimer += deltaTime;
	if (AnimationTimer >= FrameInterval)
	{
		CurrentFrame = (CurrentFrame + 1) % 2;
		AnimationTimer = 0.0f;
	}
}
