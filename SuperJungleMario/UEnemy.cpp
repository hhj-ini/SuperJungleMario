#include "UEnemy.h"
#include "UPlayer.h"
#include "ResourceManager.h"
#include "UGameLogic.h"
#include "USoundManager.h"

//UEnemy::UEnemy()
//{
//	Velocity.x = 0.0f;
//	Velocity.y = 0.0f;
//
//	Location.x = 1.0f;
//	Location.y = -0.5f;
//	// 위치 설정 필요
//
//	width = scaleMod;
//	height = scaleMod;
//	bisMove = true;
//
//	SetState(EnemyState::ALIVE);
//
//	ObjectType = EObjectType::ENEMY;
//}

UEnemy::UEnemy(float x, float y, float w, float h)
{
	Velocity.x = 0.0f;
	Velocity.y = 0.0f;
	Location.x = x;
	Location.y = y;
	width = w * scaleMod;
	height = h * scaleMod;

	bisMove = true;
	SetState(EnemyState::ALIVE);
	ObjectType = EObjectType::ENEMY;
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

		//DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

		renderer.RenderPrimitive(pBuffer, num);
	}
}

bool UEnemy::CollisionCheck(UPrimitive* other)
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
	if (overlapX > 0 && overlapY > 0)
	{
		switch (other->ObjectType)
		{
		case EObjectType::BOX: // 박스와 충돌 시 처리
		{
			if (overlapX > overlapY) { //y축방향으로 충돌시 y속도 0으로 처리		
				float onBoxDistance = std::fabs(Location.y - (other->Location.y + scaleMod));
				float overlapOnTheBox = sumHalfHeight - onBoxDistance;

				if (overlapOnTheBox > 0.0f)	// 1. 박스 위를 걷고있는 경우
				{
					Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
					Velocity.y = 0.0f;
					break;
				}
			}
			else { // x축방향으로 충돌시 x속도 0으로 처리
				float rightBoxDistance = std::fabs(Location.x - (other->Location.x + scaleMod));
				float overlapRightSideBox = sumHalfHeight - rightBoxDistance;

				if (overlapRightSideBox > 0.0f)	// 오른쪽에서 충돌
				{
					Location.x = other->Location.x + (other->width / 2.0f) + (width / 2.0f);
					Velocity.x *= -1.0f;
					break;
				}
				else
				{
					Location.x = other->Location.x - ((other->width / 2.0f) + (width / 2.0f));
					Velocity.x *= -1.0f;
					break;
				}
			}
			break;
		}
		case EObjectType::PIPE: // 파이프충돌처리
		{
			if (overlapX > overlapY) { //y축방향으로 충돌시 y속도 0으로 처리		
				float onBoxDistance = std::fabs(Location.y - (other->Location.y + scaleMod));
				float overlapOnTheBox = sumHalfHeight - onBoxDistance;

				if (overlapOnTheBox > 0.0f)	// 1. 박스 위를 걷고있는 경우
				{
					Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
					Velocity.y = 0.0f;
					break;
				}
			}
			else { // x축방향으로 충돌시 x속도 0으로 처리
				float rightBoxDistance = std::fabs(Location.x - (other->Location.x + scaleMod));
				float overlapRightSideBox = sumHalfHeight - rightBoxDistance;

				if (overlapRightSideBox > 0.0f)	// 오른쪽에서 충돌
				{
					Location.x = other->Location.x + (other->width / 2.0f) + (width / 2.0f);
					Velocity.x *= -1.0f;
					break;
				}
				else
				{
					Location.x = other->Location.x - ((other->width / 2.0f) + (width / 2.0f));
					Velocity.x *= -1.0f;
					break;
				}
			}
			break;
		}
		default:
			break;
		return false;
		}
	}
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
		Velocity.x = 0.01f;
		break;

	case EnemyState::DEAD:
		eState = EnemyState::DEAD;
		break;
	}
}

void UEnemy::OnDeath(UPlayer* player)
{
	if (eState == EnemyState::ALIVE)
	{
		SetState(EnemyState::DEAD);
		SoundManager->PlaySoundResource(SoundBufferMap[L"GoombaDead"]);
		bIsActive = false;
		// 몬스터가 죽으면 점수 올라감
		UGameLogic::GetInstance().addScore(100, Location.x, Location.y);
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

void UEnemy::SetSoundResource(USoundManager* soundManager)
{
	SoundManager = soundManager;

	std::wstring soundName = L"GoombaDead";	//설정한 이름으로 접근 가능
	SoundBufferMap[soundName] = ResourceManager::GetInstance().GetSoundResource(L"Resource\\Sound\\GoombaDead.wav", soundManager);
	SoundManager->ElaryLoadSoundResource(SoundBufferMap[soundName]);

}
