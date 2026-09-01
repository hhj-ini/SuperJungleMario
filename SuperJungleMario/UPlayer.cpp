#pragma once
#include "UPlayer.h"
#include <math.h>


UPlayer::UPlayer()
{
	Location.x = ((float)(rand() % 180 - 90)) * 0.01f;
	Location.y = ((float)(rand() % 180 - 90)) * 0.01f;

	Velocity.x = 0.001f;
	Velocity.y = 0.001f;
}

UPlayer::~UPlayer()
{
}

void UPlayer::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	/*renderer.UpdateConstantBuffer(Location, Radius);
	renderer.RenderPrimitive(pBuffer, num);*/
<<<<<<< Updated upstream
=======
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
>>>>>>> Stashed changes
}

//bool UPlayer::CollisionCheck(UPrimitive* other)
//{
//	if (this == other)
//	{
//		return false;
//	}
//
//	UPlayer* Other = nullptr;
//	if (!(Other = dynamic_cast<UPlayer*>(other)))
//	{
//		return false;
//	}
//
//	float xdist = Location.x - Other->Location.x;
//	float ydist = Location.y - Other->Location.y;
//
//	float dist = sqrtf((xdist * xdist) + (ydist * ydist));
//	float radiustest = (Radius * scaleMod) + (Other->Radius * scaleMod);
//
//	if (dist <= radiustest && dist > 0.0f)
//	{
//		float nx = xdist / dist;
//		float ny = ydist / dist;
//
//		float overlap = radiustest - dist;
//		Location.x += nx * overlap / 2;
//		Location.y += ny * overlap / 2;
//
//		Other->Location.x -= nx * overlap / 2;
//		Other->Location.y -= ny * overlap / 2;
//
//		// 상대속도 벡터
//		float rvx = Velocity.x - Other->Velocity.x;
//		float rvy = Velocity.y - Other->Velocity.y;
//
//		// 투영
//		float vreln = (rvx * nx) + (rvy * ny);
//		if (vreln > 0.0f)
//		{
//			return false;
//		}
//
//		float e = 0.3f;	// 반발계수
//		float impulseScalar = -1 * (1 + e) * vreln / ((1 / Mass) + (1 / Other->Mass));
//
//		Velocity.x = Velocity.x + (impulseScalar / Mass) * nx;
//		Velocity.y = Velocity.y + (impulseScalar / Mass) * ny;
//
//		Other->Velocity.x = Other->Velocity.x - (impulseScalar / Other->Mass) * nx;
//		Other->Velocity.y = Other->Velocity.y - (impulseScalar / Other->Mass) * ny;
//
//		return true;
//	}
//
//	return false;
//}


void UPlayer::Move()
{

	Velocity.x = 0.0f;

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

	// Velocity.y -= gravity * deltaTime;

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

