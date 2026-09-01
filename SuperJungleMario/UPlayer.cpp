#pragma once
#include "UPlayer.h"
#include <math.h>
#include "ResourceManager.h"

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

//bool UPlayer::CollisionCheck(UPrimitive* other)
//{
//	if (this == other)
//	{
//		return false;
//	}
//
//	/*UPlayer* Other = nullptr;
//	if (!(Other = dynamic_cast<UPlayer*>(other)))
//	{
//		return false;
//	}*/
//
//	return false;
//}

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
		//Velocity.y = 0.0f;

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
			Velocity.y += 0.05f;
			bIsGrounded = false;
		}


		Location.x += Velocity.x * deltaTime;
		Location.y += Velocity.y * deltaTime;

		// 바닥 착지 처리 필요(충돌)
		
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

