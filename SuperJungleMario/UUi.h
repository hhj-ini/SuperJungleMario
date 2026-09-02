#pragma once
#include "SuperJungleMario.h"
#include "URenderer.h"
#include "UGameLogic.h"

extern POINT charPositions[];

extern char charList[];

extern POINT charPositionsBlack[];

extern char charListBlack[];

extern POINT charPositionsEnd[];

extern char charListEnd[];

extern POINT charPositionsStart[];

extern char charListStart[];

extern POINT charPositionsOver[];

extern char charListOver[];

class UUi
{
public:
	FVertexUI* UIVertex;
	DirectX::XMFLOAT2 NDCoord;
	DirectX::XMFLOAT4 rgba;
	DirectX::XMFLOAT4 uv;

	enum class EScoreList
	{
		NONE = 0,
		s50,
		s100,
		s200,
		s1000,
	};
public:
	UUi()
		: UIVertex(nullptr)
		, NDCoord(0.0f, 0.0f)
		, rgba(1, 1, 1, 1)
		, uv(0, 0, 0, 0)
	{
	}

	UUi(FVertexUI* UIVertex, DirectX::XMFLOAT2 NDCoord = DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4 rgba = DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT4 uv = DirectX::XMFLOAT4(1, 1, 0, 0));

	void setNDCoord(DirectX::XMFLOAT2 NDCoord);

	void Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight, DirectX::XMFLOAT2 NDCoord);

	static DirectX::XMFLOAT4 Translate(char input);

	void RenderGameStart(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight);

	static void UpdateGameTime(int currentTime);

	static void UpdateScoreUI(int score);

	static void UpdateCoinUI(int coin);

	static void UpdateLifeUI(int life);

	static void UpdateFinalScoreUI(int score);

	static void UpdateOverScoreUI(int score);

	void UpdateUV(char* charList, int index);

	void UpdateFloatingUV(int index);

	static DirectX::XMFLOAT4 TranslateUV(char input);

	static DirectX::XMFLOAT4 TranslateFloatingUV(int score);
};

struct FFloatingScore
{
	UUi* floatingUUi;
	double createdTime;
	int displayScore;
};