#pragma once
#include "UPlayer.h"
#include "UEnemy.h"
#include "UMushroom.h"
#include <math.h>
#include "ResourceManager.h"
#include <cmath>
#include "UProjectile.h"


UPlayer::UPlayer()
{
	Location.x = 0.0f;
	Location.y = 0.0f;
	Location.z = 0.0f;

	Velocity.x = 0.0f;
	Velocity.y = 0.0f;

	bBigMario = false;
	bFireMario = false;
	bIsGrounded = false;
	bAttacking = false;

	Hp = 1;
	width = scaleMod;
	height = scaleMod;
	bFacingLeft = false;
	ObjectType = EObjectType::PLAYER;

	CurrentFrame = 0;
	AnimationTimer = 0.0f;

	DamageTimer = 0.0f;
	FireTimer = 0.0f;
	bShotFireRequest = false;
}

UPlayer::~UPlayer()
{
}

void UPlayer::Tick()
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
		L"Resource\\Mario\\BigMario6.png",

		L"Resource\\Mario\\FireBigMario1.png",
		L"Resource\\Mario\\FireBigMario2.png",
		L"Resource\\Mario\\FireBigMario3.png",
		L"Resource\\Mario\\FireBigMario4.png",
		L"Resource\\Mario\\FireBigMario5.png",
		L"Resource\\Mario\\FireBigMario6.png",

		L"Resource\\Mario\\FireAttackMario1.png",
		L"Resource\\Mario\\FireAttackMario2.png"
	};

	for (int i = 0; i < 20; ++i)
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

void UPlayer::UpdateVelocity(bool bGravity)
{
	UBall::UpdateVelocity(bGravity);
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
		{
			if (overlapX > overlapY) { //y축방향으로 충돌시 y속도 0으로 처리		
				float onBoxDistance = std::fabs(Location.y - (other->Location.y + scaleMod));
				float overlapOnTheBox = sumHalfHeight - onBoxDistance;

				if (overlapOnTheBox > 0.0f)	// 1. 박스 위를 걷고있는 경우
				{
					bIsGrounded = true;
					Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
					Velocity.y = 0.0f;
					break;
				}
				else 	// 2. 박스 아래에서 충돌된 경우
				{
					UBox* bp = dynamic_cast<UBox*>(other);
					
					Location.y = other->Location.y - ((other->height / 2.0f) + (height / 2.0f));	
					//if (bp && (UBox::EBoxType::BRICK == bp->BoxType || UBox::EBoxType::QUESTION == bp->BoxType))
					//{
					//	// 의도적으로 overlap되도록 함
					//	//float overlapAcceptDegree = 0.01f;
					//	//Location.y += overlapAcceptDegree;
					//}
					
					Velocity.y *= -0.5f;
					break;
				}
			}
			else { // x축방향으로 충돌시 x속도 0으로 처리
				float rightBoxDistance = std::fabs(Location.x - (other->Location.x + scaleMod));
				float overlapRightSideBox = sumHalfHeight - rightBoxDistance;

				if (overlapRightSideBox > 0.0f)	// 오른쪽에서 충돌
				{
					Location.x = other->Location.x + (other->width / 2.0f) + (width / 2.0f);
					Velocity.x = 0.0f;
					break;
				}
				else
				{
					Location.x = other->Location.x - ((other->width / 2.0f) + (width / 2.0f));
					Velocity.x = 0.0f;
					break;
				}

				bIsGrounded = false;
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
					bIsGrounded = true;
					Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
					Velocity.y = 0.0f;
					break;
				}
				else 	// 2. 박스 아래에서 충돌된 경우
				{
					Location.y = other->Location.y - ((other->height / 2.0f) + (height / 2.0f)) + 0.01f;	// 의도적으로 overlap되도록 함
					Velocity.y *= -0.5f;
					break;
				}
			}
			else { // x축방향으로 충돌시 x속도 0으로 처리
				float rightBoxDistance = std::fabs(Location.x - (other->Location.x + scaleMod));
				float overlapRightSideBox = sumHalfHeight - rightBoxDistance;

				if (overlapRightSideBox > 0.0f)	// 오른쪽에서 충돌
				{
					Location.x = other->Location.x + (other->width / 2.0f) + (width / 2.0f);
					Velocity.x = 0.0f;
					break;
				}
				else
				{
					Location.x = other->Location.x - ((other->width / 2.0f) + (width / 2.0f));
					Velocity.x = 0.0f;
					break;
				}

				bIsGrounded = false;
			}
			break;
		}
		case EObjectType::ENEMY:
			if (UEnemy* enemy = dynamic_cast<UEnemy*>(other))
			{
				// 플레이어가 적에게 공격받을 시 적도 사라지는 버전
				float enemyTop = enemy->GetPosition().y + enemy->GetHeight() / 2.0f;
				float prePlayerBottom = GetPreviousPosition().y - GetHeight() / 2.0f;
				if (prePlayerBottom >= enemyTop && GetVelocity().y < 0.0f)
				{
					enemy->OnDeath(this);
				}
				else
				{
					TakeDamage();
				}
			}
			break; 
		case EObjectType::MUSHROOM:
			if (Hp == 1)
			{
				++Hp;
				Grow();
			}
			break;
		case EObjectType::FLOWER:
			if (!bFireMario)
			{
				FireMario();
			}
			break;
		case EObjectType::PROJECTILE:
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
		if (bIsGrounded && (GetAsyncKeyState(VK_SPACE) & 0x8000))
		{
			Velocity.y = 0.05f;
			bIsGrounded = false;
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			Velocity.x = -0.01f;
			bFacingLeft = true;
			// bIsGrounded = false;
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			Velocity.x = 0.01f;
			bFacingLeft = false;
			// bIsGrounded = false;
		}		
		
		PreviousLocation = Location;
		Location.x += Velocity.x * deltaTime;
		Location.y += Velocity.y * deltaTime;

		if (GetAsyncKeyState('D') & 0x8000 && bFireMario)
		{
			RequestFire();
		}
	}
}

void UPlayer::TakeDamage()
{
	if (DamageTimer > 0.0f)
	{
		return;
	}

	--Hp;

	if (Hp == 2)
	{
		bFireMario = false;
		CurrentFrame = bFacingLeft ? 8 : 6;

		DamageTimer = DamageInvincibleTime;
		return;
	}

    if (Hp == 1)
	{
		Shrink();
		DamageTimer = DamageInvincibleTime;
		return;
	}

	Hp = 0;
	SetState(PlayerState::DEAD);
}

void UPlayer::Grow()
{
	bBigMario = true;
	
	float oldHeight = height;

	width = scaleMod;
	height = scaleMod * 2.0f;
	Location.y += (height - oldHeight) / 2.0f;
}

void UPlayer::Shrink()
{
	bBigMario = false;
	float oldHeight = height;

	width = scaleMod;
	height = scaleMod;
	Location.y -= (oldHeight - height) / 2.0f;
}

void UPlayer::UpdateAnimation(float deltaTime)
{
	if (DamageTimer > 0.0f)
	{
		DamageTimer -= deltaTime;
	}
	if (FireTimer > 0.0f)
	{
		FireTimer -= deltaTime;
		if (FireTimer <= 0.0f)
		{
			bAttacking = false;
		}
	}

	int BaseFrame = GetBaseFrame();

	AnimationTimer += deltaTime;
	if (AnimationTimer >= FrameInterval)
	{
		if (!bAttacking)
		{
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
		}
		else
		{
			CurrentFrame = bFacingLeft ? 19 : 18;
		}
		AnimationTimer = 0.0f;
	}
}

void UPlayer::Respawn()
{
	UPlayer::SetState(UPlayer::PlayerState::ALIVE);
	
}

int UPlayer::GetBaseFrame() const
{
	if (bFireMario)
		return 12; // 불 마리오

	if (!bFireMario && bBigMario)
		return 6;  // 큰 마리오

	return 0;      // 작은 마리오
}

void UPlayer::FireMario()
{
	if (!bFireMario)
	{
		bFireMario = true;
		CurrentFrame = bFacingLeft ? 14 : 12;
	}

	Hp = 3;
	if (!bBigMario)
	{
		Grow();
	}
}

void UPlayer::RequestFire()
{
	if (FireTimer > 0.0f)
	{
		return;
	}

	bShotFireRequest = true;
	bAttacking = true;
	

	CurrentFrame = bFacingLeft ? 19 : 18;

	FireTimer = FireInterval;
}

bool UPlayer::ShotFireRequest()
{
	bool bRequested = bShotFireRequest;
	bShotFireRequest = false;
	return bRequested;
}