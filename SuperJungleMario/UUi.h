#pragma once
#include "SuperJungleMario.h"
#include "URenderer.h"

class UUi
{
public:
	FVertexUI* UIVertex;
	DirectX::XMFLOAT2 NDCoord;
	DirectX::XMFLOAT4 rgba;
	float scale; // 일단 넣어둠
public:
	UUi()
		: UIVertex(nullptr)
		, NDCoord(0.0f, 0.0f)
		, rgba(1, 1, 1, 1)
		, scale(1.0f)
	{
	}

	UUi(FVertexUI* UIVertex, DirectX::XMFLOAT2 NDCoord, DirectX::XMFLOAT4 rgba, float scale);

	void setNDCoord(DirectX::XMFLOAT2 NDCoord)
	{
		UUi::NDCoord = NDCoord;
	}

	void Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight);
};