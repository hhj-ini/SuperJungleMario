#include "UProjectile.h"
#include "ResourceManager.h"
#include "UPlayer.h"

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

    if (EProjectileState::ROLLING == CurrState)
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
        renderer.PrepareShaderResource(TextureSRVPtr[CurrentFrame]);

        DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
        renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

        renderer.RenderPrimitive(pBuffer, num);
    }

    if (EProjectileState::HIT == CurrState)
    {

    }

    /*DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
    renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

    renderer.RenderPrimitive(pBuffer, num);*/
}

bool UProjectile::CollisionCheck(UPrimitive * other)
{
    if (EProjectileState::ROLLING != CurrState)
    {   // ROLLING 중 일때만 콜리전 활성화
        return false;
    }

    // 부딪힌 상대가 적(enemy)이라면 상대에게 데미지 입히기
    //if (true/* 부딪힌 상대가 적*/)
    //{
    //    SetState(EProjectileState::HIT);
    //}

    //if (true/* 부딪힌 상대가 벽, 땅*/)
    //{
    //    SetState(EProjectileState::WAITING);
    //}
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
                Location.y = other->Location.y + (other->height / 2.0f) + (height / 2.0f);
                Velocity.y = 0;
                break;
            }
            //else { // x축방향으로 충돌시 x속도 0으로 처리
            //	Location.x = other->Location.x + (other->width / 2.0f) + (width / 2.0f);
            //	Velocity.x = 0;
            //}
        case EObjectType::PLAYER:
            break;
        case EObjectType::MUSHROOM:
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
    //Location.x += Velocity.x;
}

void UProjectile::SetState(EProjectileState InState)
{
    switch (InState)
    {
    case EProjectileState::WAITING:
        CurrState = EProjectileState::WAITING;
        break;
    case EProjectileState::ROLLING:
        // 애니메이션 재생 필요

        // 속도, 위치 초기화
        /*Location.x = Owner->Location.x;
        Location.y = Owner->Location.y;

        Velocity.x = 0.1f;
        Velocity.y = -0.1f;*/

        CurrState = EProjectileState::ROLLING;

        break;
    case EProjectileState::HIT:
        // 애니메이션 재생 필요
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
		CurrState = EProjectileState::ROLLING;

		bActive = true;
		bisMove = true;
        LifeTime = 0; // 초기화

		Location.x = PlayerLocation.x + (bFacingLeft ? -1.0f : 1.0f) * (playerWidth / 2.0f + width / 2.0f);
		Location.y = PlayerLocation.y;

        Velocity.x = bFacingLeft ? -0.01f : 0.01f;

    return true;
}

void UProjectile::DeactivateProjectile()
{
    bActive = false;
	bisMove = false;
	CurrState = EProjectileState::WAITING;
    Velocity.x = 0.0f;
    Velocity.y = 0.0f;
}

void UProjectile::UpdateAnimation(float deltaTime)
{
    AnimationTimer += deltaTime;
    if (AnimationTimer >= FrameInterval)
    {
        CurrentFrame = (CurrentFrame + 1) % 4;
        AnimationTimer = 0.0f;
    }
}

void UProjectile::UpdateVelocity(bool bGravity)
{
    /*float gravity = (bGravity) ? GravityAmount : 0.0f;

    if (bGravity)
    {
        Velocity.y -= gravity * deltaTime;
    }*/
   
    Velocity.y -= 0.00001f;
    
    //return;
}