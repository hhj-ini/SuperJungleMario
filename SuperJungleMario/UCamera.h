#pragma once
#include <DirectXMath.h>
#include "SuperJungleMario.h"

class UCamera
{
public:
	float x = 0.0f;
	float y = 0.0f;

	void Follow(const FVector& targetLocation)	
	{
		if (targetLocation.x > x)
		{
			x = targetLocation.x;
		}

		y = 0.0f;
	}

	void Reset()
	{
		x = 0.0f;
		y = 0.0f;
	}

	DirectX::XMMATRIX GetViewMatrix() const
	{
		
		return DirectX::XMMatrixTranslation(-x, -y, 0.0f);
	}

};