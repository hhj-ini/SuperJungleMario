#include "UProjectile.h"
#include "ResourceManager.h"
#include "UPlayer.h"

void UProjectile::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
    if (EProjectileState::WAITING == CurrState)
    {
        return;
    }

    if (EProjectileState::ROLLING == CurrState)
    {
        

    }

    if (EProjectileState::HIT == CurrState)
    {

    }

    DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
    renderer.UpdateConstantBuffer(world, renderer.ViewMatrix);

    renderer.RenderPrimitive(pBuffer, num);
}

bool UProjectile::CollisionCheck(UPrimitive * other)
{
    if (EProjectileState::ROLLING != CurrState)
    {   // ROLLING 중 일때만 콜리전 활성화
        return false;
    }

    // 부딪힌 상대가 적(enemy)이라면 상대에게 데미지 입히기
    if (true/* 부딪힌 상대가 적*/)
    {
        SetState(EProjectileState::HIT);
    }

    if (true/* 부딪힌 상대가 벽, 땅*/)
    {
        SetState(EProjectileState::WAITING);
    }
    
    return false;
}

void UProjectile::SetState(EProjectileState InState)
{
    switch (InState)
    {
    case EProjectileState::WAITING:
        break;
    case EProjectileState::ROLLING:
        // 애니메이션 재생 필요

        // 속도, 위치 초기화
        Location.x = Owner->Location.x;
        Location.y = Owner->Location.y;

        Velocity.x = 1.0f;
        Velocity.y = -1.0f;

        break;
    case EProjectileState::HIT:
        // 애니메이션 재생 필요

        Velocity.x = 0.0f;
        Velocity.y = 0.0f;
         
        break;
    }
}
