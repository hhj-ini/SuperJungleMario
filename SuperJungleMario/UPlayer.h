#pragma once
#include "UBall.h"

class UPlayer : public UBall
{
public:
    bool bIsGrounded = true;

public:
    UPlayer();
    ~UPlayer() override;

    void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) override;
    void Move() override;
};