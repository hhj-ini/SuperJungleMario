#include "UProjectile.h"

void UProjectile::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
    return;
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
        break;
    case EProjectileState::HIT:
        break;
    }
}
