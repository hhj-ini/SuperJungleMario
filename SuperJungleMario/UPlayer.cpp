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
	bFacingLeft = false;
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
		//밀어내기 구현
		if (bisMove == false && other->bisMove == true) { // 속도가 0, 즉 블럭이면
			other->Location.x += (overlapX) * ((Location.x < other->Location.x) ? 1.0f : -1.0f);
			other->Location.y += (overlapY) * ((Location.y < other->Location.y) ? 1.0f : -1.0f);

			//겹치는 간격이 작은쪽의 속도를 멈춤

			if (overlapX < overlapY) {
				Velocity.x = 0.0f;
				other->Velocity.x = 0.0f;

			}
			else {
				Velocity.y = 0.0f;
				other->Velocity.y = 0.0f;
			}
		}
		else if (other->bisMove == false && bisMove == true) {

			//겹치는 간격이 작은쪽의 속도를 멈춤

			if (overlapX < overlapY) {
				Location.x += (overlapX) * ((Location.x < other->Location.x) ? -1.0f : 1.0f);
				Velocity.x = 0.0f;
				other->Velocity.x = 0.0f;

			}
			else {
				Location.y += (overlapY) * ((Location.y < other->Location.y) ? -1.0f : 1.0f);
				Velocity.y = 0.0f;
				other->Velocity.y = 0.0f;
			}
		}

		else {
			Location.x += (overlapX / 2.0f) * ((Location.x < other->Location.x) ? -1.0f : 1.0f);
			Location.y += (overlapY / 2.0f) * ((Location.y < other->Location.y) ? -1.0f : 1.0f);
			other->Location.x += (overlapX / 2.0f) * ((Location.x < other->Location.x) ? 1.0f : -1.0f);
			other->Location.y += (overlapY / 2.0f) * ((Location.y < other->Location.y) ? 1.0f : -1.0f);
		}

		return true;
	}
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
		/*else
		{
			CurrentFrame = bFacingLeft ? 2 : 0;
			AnimationTimer = 0.0f;
		}*/

		if (bIsGrounded && (GetAsyncKeyState(VK_SPACE) & 0x8000))
		{
			Velocity.y = 0.005f;
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

void UPlayer::UpdateAnimation(float deltaTime)
{
	AnimationTimer += deltaTime;
	if (AnimationTimer >= FrameInterval)
	{
		if (Velocity.x > 0.0f)
		{
			CurrentFrame = (CurrentFrame + 1) % 2; // 오른쪽 이동 시 프레임 0과 1을 번갈아가며 사용
		}
		else if (Velocity.x < 0.0f)
		{
			CurrentFrame = 2 + (CurrentFrame + 1) % 2; // 왼쪽 이동 시 프레임 2와 3을 번갈아가며 사용
		}
		else
		{
			CurrentFrame = bFacingLeft ? 2 : 0; // 정지 상태에서는 마지막 방향에 따라 프레임 설정
		}
		AnimationTimer = 0.0f;
	}
}

