#include "UProjectile.h"
#include "ResourceManager.h"
#include "UPlayer.h"
#include "UEnemy.h"
#include "UBox.h"

UProjectile::UProjectile()
{
    width = scaleMod * 0.5f;
    height = scaleMod * 0.5f;

	bActive = false;
	CurrentFrame = 0;
	AnimationTimer = 0.0f;
    LifeTime = 0;

    SetState(EProjectileState::WAITING);

    ObjectType = EObjectType::PROJECTILE;
}

void UProjectile::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
    if (EProjectileState::WAITING == CurrState)
    {
        return;
    }

    if (EProjectileState::ROLLING == CurrState || EProjectileState::HIT == CurrState)
    {
        // 렌더하기 전에 텍스쳐 바인딩
        if (!TextureSRVPtr[0])
        {
            TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\Projectile\\Projectile1.png", &renderer);
        }
        if (!TextureSRVPtr[1])
        {
            TextureSRVPtr[1] = ResourceManager::GetInstance().GetSRV(L"Resource\\Projectile\\Projectile2.png", &renderer);
        }
        if (!TextureSRVPtr[2])
        {
            TextureSRVPtr[2] = ResourceManager::GetInstance().GetSRV(L"Resource\\Projectile\\Projectile3.png", &renderer);
        }
        if (!TextureSRVPtr[3])
        {
            TextureSRVPtr[3] = ResourceManager::GetInstance().GetSRV(L"Resource\\Projectile\\Projectile4.png", &renderer);
        }
        if (!TextureSRVPtr[4])
        {
            TextureSRVPtr[4] = ResourceManager::GetInstance().GetSRV(L"Resource\\Projectile\\Projectile5.png", &renderer);
        }
        renderer.PrepareShaderResource(TextureSRVPtr[CurrentFrame]);

        DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
        renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);
        renderer.RenderPrimitive(pBuffer, num);
        
    }
}

bool UProjectile::CollisionCheck(UPrimitive * other)
{
    if (EProjectileState::ROLLING != CurrState)
    {   // ROLLING 중 일때만 콜리전 활성화
        return false;
    }

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
		case EObjectType::ENEMY:
            if (UEnemy* enemy = dynamic_cast<UEnemy*>(other))
            {
				enemy->OnDeath(dynamic_cast<UPlayer*>(Owner));
                CurrState = EProjectileState::HIT;
                DeactivateProjectile();
			}
			break;
        case EObjectType::BOX: // 박스와 충돌 시 처리
        {
			if (CollisionTimer < CollisionInterval)
			{
				break;
			}

            ++LifeTime;
            if (LifeTime > 2)
            {
                CurrState = EProjectileState::HIT;
                DeactivateProjectile();
            }

            if (overlapX > overlapY) { 	
                float onBoxDistance = std::fabs(Location.y - (other->Location.y + scaleMod));
                float overlapOnTheBox = sumHalfHeight - onBoxDistance;

                if (overlapOnTheBox > 0.0f)	// 1. 박스 위를 걷고있는 경우
                {
                    Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
                    if (Velocity.y > 0.3f)
                    {
                        CurrState = EProjectileState::HIT;
                        DeactivateProjectile();
                    }
                    else
                    {
                        Velocity.y = 0.01f;
                    }
                    break;
                }
                else 	// 2. 박스 아래에서 충돌된 경우
                {
                    UBox* bp = dynamic_cast<UBox*>(other);

                    Location.y = other->Location.y - ((other->height / 2.0f) + (height / 2.0f));
                    Velocity.y *= -1.0f;
                    break;
                }
            }
            else { 
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
            if (CollisionTimer < CollisionInterval)
            {
                break;
            }

            ++LifeTime;
            if (LifeTime > 2)
            {
                CurrState = EProjectileState::HIT;
                DeactivateProjectile();
            }

            if (overlapX > overlapY) { //y축방향으로 충돌시 y속도 0으로 처리		
                float onBoxDistance = std::fabs(Location.y - (other->Location.y + scaleMod));
                float overlapOnTheBox = sumHalfHeight - onBoxDistance;

                if (overlapOnTheBox > 0.0f)	// 1. 박스 위를 걷고있는 경우
                {
                    Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
                    if (Velocity.y > 0.3f)
                    {
                        CurrState = EProjectileState::HIT;
                        DeactivateProjectile();
                    }
                    else
                    {
                        Velocity.y = 0.01f;
                    }
                    break;
                }
                else 	// 2. 박스 아래에서 충돌된 경우
                {
                    Location.y = other->Location.y - ((other->height / 2.0f) + (height / 2.0f)) + 0.01f;	// 의도적으로 overlap되도록 함
                    Velocity.y *= -1.0f;
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
        }
    }
    return false;
}

void UProjectile::Move()
{
    if (EProjectileState::ROLLING != CurrState)
    {
        return;
    }
    
    this->UBall::Move();
}

void UProjectile::SetState(EProjectileState InState)
{
    switch (InState)
    {
    case EProjectileState::WAITING:
        CurrState = EProjectileState::WAITING;
        break;
    case EProjectileState::ROLLING:
        CurrState = EProjectileState::ROLLING;
        break;
    case EProjectileState::HIT:
		CurrentFrame = 4;
        CurrState = EProjectileState::HIT;

        Velocity.x = 0.0f;
        Velocity.y = 0.0f;
         
        break;
    }
}

void UProjectile::SetOwner(UPrimitive* InOwner)
{
    Owner = dynamic_cast<UPlayer*>(InOwner);
}

bool UProjectile::ActivateProjectile(FVector PlayerLocation, bool bFacingLeft, float playerWidth)
{
    if (CurrState != EProjectileState::WAITING || bActive)
    {
        return false;
    }
    CurrentFrame = 0;
    AnimationTimer = 0.0f;
    ExplosionTimer = 0.0f;

	CurrState = EProjectileState::ROLLING;

	bActive = true;
	bisMove = true;
    LifeTime = 0; // 초기화

	Location.x = PlayerLocation.x + (bFacingLeft ? -1.0f : 1.0f) * (playerWidth / 2.0f + width / 2.0f);
	Location.y = PlayerLocation.y + 0.05f;

    Velocity.x = bFacingLeft ? -0.025f : 0.025f;

    return true;
}

void UProjectile::DeactivateProjectile()
{
    CurrState = EProjectileState::HIT;
    CurrentFrame = 4;

    bActive = false;
	bisMove = false;
    
    Velocity.x = 0.0f;
    Velocity.y = 0.0f;

    ExplosionTimer = 0.0f;
}

void UProjectile::UpdateAnimation(float deltaTime)
{
    if (CurrState == EProjectileState::HIT)
    {
        CurrentFrame = 4;
        ExplosionTimer += deltaTime;

        if (ExplosionTimer >= 0.1f)
        {
            CurrState = EProjectileState::WAITING;
            bActive = false;
            bisMove = false;
        }

        return;
    }

    AnimationTimer += deltaTime;
    CollisionTimer += deltaTime;
    if (AnimationTimer >= FrameInterval)
    {
        CurrentFrame = (CurrentFrame + 1) % 4;
        AnimationTimer = 0.0f;
    }
}

void UProjectile::UpdateVelocity(bool bGravity)
{
    Velocity.y -= 0.00015f;
    
}