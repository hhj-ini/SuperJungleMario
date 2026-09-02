#pragma once
#include "SuperJungleMario.h"
#include "URenderer.h"

extern POINT charPositions[];

extern char charList[];

extern POINT charPositionsBlack[];

extern char charListBlack[];

extern POINT charPositionsEnd[];

extern char charListEnd[];

extern POINT charPositionsStart[];

extern char charListStart[];

class UUi
{
public:
	FVertexUI* UIVertex;
	DirectX::XMFLOAT2 NDCoord;
	DirectX::XMFLOAT4 rgba;
	DirectX::XMFLOAT4 uv;
	float scale; // 일단 넣어둠
public:
	UUi()
		: UIVertex(nullptr)
		, NDCoord(0.0f, 0.0f)
		, rgba(1, 1, 1, 1)
		, uv(0, 0, 0, 0)
		, scale(1.0f)
	{
	}

	UUi(FVertexUI* UIVertex, DirectX::XMFLOAT2 NDCoord, DirectX::XMFLOAT4 rgba, DirectX::XMFLOAT4 uv, float scale);

	void setNDCoord(DirectX::XMFLOAT2 NDCoord);

	void Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight, DirectX::XMFLOAT2 NDCoord);

	static DirectX::XMFLOAT4 Translate(char input);

	void RenderGameStart(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight);

	static void UpdateGameTime(int currentTime);

	static void UpdateScoreUI(int score);

	static void UpdateCoinUI(int coin);

	static void UpdateLifeUI(int life);

	void UpdateUV(int index);

	void UpdateUVBlack(int index);
};