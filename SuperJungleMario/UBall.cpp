#pragma once
#include "UBall.h"
#include <math.h>


UBall::UBall()
{
	Location.x = ((float)(rand() % 180 - 90)) * 0.01f;
	Location.y = ((float)(rand() % 180 - 90)) * 0.01f;

	Velocity.x = ((float)(rand() % 100 - 50)) * 0.0001f;
	Velocity.y = ((float)(rand() % 100 - 50)) * 0.001f;

	Radius = ((float)(rand() % 100 + 10)) * 0.01f;
	Mass = Radius * Radius * 3.14f;
	Index = TotalNumBalls++;

	width = Radius * 2.0 * scaleMod;
	height = Radius * 2.0f * scaleMod;
}

UBall::~UBall()
{
	--TotalNumBalls;
}


void UBall::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	renderer.UpdateConstantBuffer(Location, Radius);
	renderer.RenderPrimitive(pBuffer, num);
}

//bool UBall::CollisionCheck(UPrimitive* other)
//{
//	if (this == other)
//	{
//		return false;
//	}
//
//	UBall* Other = nullptr;
//	if (!(Other = dynamic_cast<UBall*>(other)))
//	{
//		return false;
//	}
//
//	// 피킹 중일때는 충돌 연산 처리 안하도록 함
//	if (bIsHold || Other->bIsHold)
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


void UBall::Move()
{
	if (bIsHold)
	{
		return;
	}
	Location.x += Velocity.x * deltaTime;
	Location.y += Velocity.y * deltaTime;
	Location.z += Velocity.z * deltaTime;

	float renderRadius = Radius * scaleMod;
	if (Location.x <= leftBorder + renderRadius)
	{
		Velocity.x *= -1.0f;
		Location.x = leftBorder + renderRadius;
	}
	if (Location.x >= rightBorder - renderRadius)
	{
		Velocity.x *= -1.0f;
		Location.x = rightBorder - renderRadius;
	}
	if (Location.y <= bottomBorder + renderRadius)
	{
		Velocity.y *= -1.0f;
		Location.y = bottomBorder + renderRadius;
	}
	if (Location.y >= topBorder - renderRadius)
	{
		Velocity.y *= -1.0f;
		Location.y = topBorder - renderRadius;
	}
}

void UBall::UpdateVelocity(bool bGravity, bool bFriction)
{
	if (bIsHold)
	{
		return;
	}

	float gravity = (bGravity) ? GravityAmount : 0.0f;
	float damping = (bFriction) ? DampingAmount : 1.0f;

	Velocity.x *= damping;
	Velocity.y *= damping;

	// 무한히 댐핑값 곱해지는 것을 방지
	if (0.00001f > Velocity.x && -0.00001f < Velocity.x)
	{
		Velocity.x = 0.0f;
	}
	if (0.00001f > Velocity.y && -0.00001f < Velocity.y)
	{
		Velocity.y = 0.0f;
	}


	Velocity.y -= gravity * deltaTime;
}

