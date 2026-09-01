#pragma once
#include "UPlayer.h"
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
	Life = 1;
	width = scaleMod;
	height = scaleMod;
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

	if (!TextureSRVPtr[0])
	{
		TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Mario\\Mario1.png", &renderer);
	}
	renderer.PrepareShaderResource(TextureSRVPtr[0]);

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
				break;
			}
			else { // x축방향으로 충돌시 x속도 0으로 처리
				Location.x = other->Location.x + (other->width / 2.0f) + (width / 2.0f);
				Velocity.x = 0;
			}
			return true;
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
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			Velocity.x += 0.01f;
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
	--Life;

	if (Life >= 1)
	{
		// 작아지는 로직?
	}
	else if (Life == 0)
	{
		SetState(PlayerState::DEAD);
	}

}

