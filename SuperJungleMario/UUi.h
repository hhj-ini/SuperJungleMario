#pragma once
#include "SuperJungleMario.h"
#include "URenderer.h"

class UUi
{
public:
	FVertexUI* UIVertex;
	FNDCoordinate NDCoord;
	float scale; // 일단 넣어둠
public:
	UUi()
		: UIVertex(nullptr)
		, NDCoord(0.0f, 0.0f)
		, scale(1.0f)
	{
	}

	UUi(FVertexUI* UIVertex, FNDCoordinate NDCoord, float scale);

	void setNDCoord(FNDCoordinate NDCoord)
	{
		UUi::NDCoord = NDCoord;
	}

	void Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight);
};