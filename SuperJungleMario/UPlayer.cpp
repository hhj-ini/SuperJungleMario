#pragma once
#include "UPlayer.h"
#include <math.h>


UPlayer::UPlayer()
{
	Location.x = 0.0f;
	Location.y = 0.0f;
	Location.z = 0.0f;

	Velocity.x = 0.0f;
	Velocity.y = 0.0f;

	bIsGrounded = true;
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
		Radius * scaleMod,
		Radius * scaleMod,
		Radius * scaleMod
	);

	XMMATRIX translation = XMMatrixTranslation(
		Location.x,
		Location.y,
		Location.z
	);

	XMMATRIX world = scale * translation;

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


void UPlayer::Move()
{

	Velocity.x = 0.0f;
	Velocity.y = 0.0f;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		Velocity.x -= 0.1f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		Velocity.x += 0.1f;
	}

	if (bIsGrounded && (GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		Velocity.y += 0.2f;
		bIsGrounded = false;
	}

	
	Location.x += Velocity.x * deltaTime;
	Location.y += Velocity.y * deltaTime;

	//Velocity.y -= gravity * deltaTime;

	// 바닥 착지 처리 필요(충돌)
}

//void UBall::UpdateVelocity(bool bGravity, bool bFriction)
//{
//	if (bIsHold)
//	{
//		return;
//	}
//
//	float gravity = (bGravity) ? GravityAmount : 0.0f;
//	float damping = (bFriction) ? DampingAmount : 1.0f;
//
//	Velocity.x *= damping;
//	Velocity.y *= damping;
//
//	// 무한히 댐핑값 곱해지는 것을 방지
//	if (0.00001f > Velocity.x && -0.00001f < Velocity.x)
//	{
//		Velocity.x = 0.0f;
//	}
//	if (0.00001f > Velocity.y && -0.00001f < Velocity.y)
//	{
//		Velocity.y = 0.0f;
//	}
//
//
//	Velocity.y -= gravity * deltaTime;
//}

