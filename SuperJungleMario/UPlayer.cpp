#pragma once
#include "UPlayer.h"
#include "UEnemy.h"
#include "UMushroom.h"
#include <math.h>
#include "ResourceManager.h"
#include <cmath>


UPlayer::UPlayer()
{
	Location.x = 0.0f;
	Location.y = 0.0f;
	Location.z = 0.0f;

	Velocity.x = 0.0f;
	Velocity.y = 0.0f;

	bIsGrounded = true;
	bBigMario = false;
	Hp = 1;
	width = scaleMod;
	height = scaleMod;
	bFacingLeft = false;
	ObjectType = EObjectType::PLAYER;
}

UPlayer::~UPlayer()
{
}

void UPlayer::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	/*renderer.UpdateConstantBuffer(Location, Radius);
	renderer.RenderPrimitive(pBuffer, num);*/

	using namespace DirectX;

	XMMATRIX scale = XMMatrixScaling(
		width,
		height,
		1.0f
	);

	XMMATRIX translation = XMMatrixTranslation(
		Location.x,
		Location.y,
		Location.z
	);

	XMMATRIX world = scale * translation;

	/*if (!TextureSRVPtr[0])
	{
		TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Mario\\Mario1.png", &renderer);
	}
	if (!TextureSRVPtr[1])
	{
		TextureSRVPtr[1] = ResourceManager::GetInstance().GetSRV(L"Resource\\Mario\\Mario2.png", &renderer);
	}
	if (!TextureSRVPtr[2])
	{
		TextureSRVPtr[2] = ResourceManager::GetInstance().GetSRV(L"Resource\\Mario\\Mario3.png", &renderer);
	}
	if (!TextureSRVPtr[3])
	{
		TextureSRVPtr[3] = ResourceManager::GetInstance().GetSRV(L"Resource\\Mario\\Mario4.png", &renderer);
	}
	if (!TextureSRVPtr[4])
	{
		TextureSRVPtr[4] = ResourceManager::GetInstance().GetSRV(L"Resource\\Mario\\Mario5.png", &renderer);
	}
	if (!TextureSRVPtr[5])
	{
		TextureSRVPtr[5] = ResourceManager::GetInstance().GetSRV(L"Resource\\Mario\\Mario6.png", &renderer);
	}*/

	const wchar_t* MarioTextures[] =
	{
		L"Resource\\Mario\\Mario1.png",
		L"Resource\\Mario\\Mario2.png",
		L"Resource\\Mario\\Mario3.png",
		L"Resource\\Mario\\Mario4.png",
		L"Resource\\Mario\\Mario5.png",
		L"Resource\\Mario\\Mario6.png",

		L"Resource\\Mario\\BigMario1.png",
		L"Resource\\Mario\\BigMario2.png",
		L"Resource\\Mario\\BigMario3.png",
		L"Resource\\Mario\\BigMario4.png",
		L"Resource\\Mario\\BigMario5.png",
		L"Resource\\Mario\\BigMario6.png"
	};

	for (int i = 0; i < 12; ++i)
	{
		if (!TextureSRVPtr[i])
		{
			TextureSRVPtr[i] =
				ResourceManager::GetInstance().GetSRV(MarioTextures[i], &renderer);
		}
	}

	renderer.PrepareShaderResource(TextureSRVPtr[CurrentFrame]);

	renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);
	renderer.RenderPrimitive(pBuffer, num);

}

bool UPlayer::CollisionCheck(UPrimitive* other)
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
		case EObjectType::BOX: // 박스와 충돌 시 처리
			if (overlapX > overlapY) { //y축방향으로 충돌시 y속도 0으로 처리
				bIsGrounded = true;
				Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
				Velocity.y = 0;
			}
			else { // x축방향으로 충돌시 x속도 0으로 처리
				Location.x = other->Location.x + (other->width / 2.0f) + (width / 2.0f);
				Velocity.x = 0;
			}
			break;
		case EObjectType::ENEMY:
			if (UEnemy* enemy = dynamic_cast<UEnemy*>(other))
			{
				if (Location.y > enemy->GetPosition().y + 0.03f)
				{
					enemy->OnDeath(this);
				}
				else
				{
					TakeDamage(1);
				}
			}
			break;
		case EObjectType::MUSHROOM:
			if (Hp == 1)
			{
				Grow();
			}
			break;

		}
	}
	return false;
}


void UPlayer::SetState(UPlayer::PlayerState InState)
{
	switch (InState)
	{
	case PlayerState::ALIVE:
		pState = PlayerState::ALIVE;
		break;
	case PlayerState::DEAD:
		pState = PlayerState::DEAD;
		break;
	}
}


void UPlayer::Move()
{
	if (pState == PlayerState::ALIVE)
	{
		Velocity.x = 0.0f;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			Velocity.x -= 0.01f;
			bFacingLeft = true;
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			Velocity.x += 0.01f;
			bFacingLeft = false;
		}

		if (bIsGrounded && (GetAsyncKeyState(VK_SPACE) & 0x8000))
		{
			Velocity.y = 0.05f;
			bIsGrounded = false;
		}

		Location.x += Velocity.x * deltaTime;
		Location.y += Velocity.y * deltaTime;
	}
}

void UPlayer::SetVelocityY(float y)
{
	Velocity.y = y;
}

void UPlayer::TakeDamage(int damage)
{
	--Hp;

	if (Hp >= 1)
	{
		Shrink();
	}
	else if (Hp == 0)
	{
		SetState(PlayerState::DEAD);
	}
}

void UPlayer::Grow()
{
	++Hp;

	if (Hp == 2)
	{
		bBigMario = true;
	}

	float oldHeight = height;

	width *= 1.0f;
	height *= 2.0f;
	Location.y += (height - oldHeight) / 2.0f;
	
}

void UPlayer::Shrink()
{
	--Hp;

	float oldHeight = height;

	width *= 0.7f;
	height *= 0.5f;
	Location.y += (oldHeight - height) / 2.0f;
}

void UPlayer::UpdateAnimation(float deltaTime)
{
	AnimationTimer += deltaTime;
	if (AnimationTimer >= FrameInterval)
	{
		int BaseFrame = bBigMario ? 6 : 0;

		if (!bIsGrounded)
		{
			CurrentFrame = BaseFrame + (bFacingLeft ? 5 : 4);
		}
		else if (Velocity.x > 0.0f)
		{
			CurrentFrame = BaseFrame + (CurrentFrame + 1) % 2;
		}
		else if (Velocity.x < 0.0f)
		{
			CurrentFrame = BaseFrame + 2 + (CurrentFrame + 1) % 2;
		}
		else
		{
			CurrentFrame = BaseFrame + (bFacingLeft ? 2 : 0);
		}
		AnimationTimer = 0.0f;
	}
}

void UPlayer::Respawn()
{
	UPlayer::SetState(UPlayer::PlayerState::ALIVE);
	
}