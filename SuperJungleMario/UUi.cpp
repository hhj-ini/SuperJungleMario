#include "UUi.h"

POINT charPositions[] =
{
	{ 84, 49 },{ 119, 45 },{ 158, 49 },{ 188, 45 },{ 227, 49 },
	{ 613, 47 },{ 648, 45 },{ 683, 45 },{ 712, 45 },{ 747, 42 },
	{ 846, 47 },{ 871, 42 },{ 905, 45 },{ 940, 42 },
	{ 84, 84 },{ 119, 84 },{ 155, 84 },{ 190, 84 },{ 226, 84 },{ 261, 84 },
	{ 435, 88 },{ 475, 84 },{ 510, 84 },
	{ 638, 84 },{ 683, 90 },{ 724, 84 },
	{ 871, 84 },{ 905, 84 },{ 940, 84 }
};

char charList[] =
{
	'M', 'A', 'R', 'I', 'O',
	'W', 'O', 'R', 'L', 'D',
	'T', 'I', 'M', 'E',
	'0', '0', '0', '3', '0', '0',
	'X', '0', '1',
	'1', '/', '1',
	'.', '.', '.'
};

POINT charPositionsStart[] =
{
	{ 355, 352 }, { 390, 350 }, { 425, 350 }, { 454, 349 }, { 489, 346 },
	{ 570, 348 }, { 610, 350 }, { 643, 347 },
	{ 519, 505 }, { 614, 500 },
};

char charListStart[] =
{
	'W', 'O', 'R', 'L', 'D',
	'1', '/', '1',
	'X', '3',
};

UUi::UUi(FVertexUI* UIVertex, DirectX::XMFLOAT2 NDCoord, DirectX::XMFLOAT4 rgba, DirectX::XMFLOAT4 uv, float scale)
	:UIVertex(UIVertex)
	, NDCoord(NDCoord)
	, rgba(rgba)
	, uv(uv)
	, scale(scale)
{
}

void UUi::Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight)
{
	renderer.UpdateUI(UUi::NDCoord, vertexBuffer, UIWidth, UIHeight, UUi::uv, UUi::rgba);
	renderer.RenderUI(vertexBuffer, numVertices);
}

//void UUi::RenderGameStart(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight)
//{
//	renderer.UpdateUI(DirectX::XMFLOAT2(0, 0), vertexBuffer, UIWidth, UIHeight, UUi::uv, UUi::rgba);
//	renderer.RenderUI(vertexBuffer, numVertices);
//}


void UUi::setNDCoord(DirectX::XMFLOAT2 NDCoord)
{
	UUi::NDCoord = NDCoord;
}

void UUi::UpdateGameTime(int currentTime)
{
	if (currentTime < 0)
	{
		// game over 로직 차후 구현
	}
	else if (currentTime < 401)
	{
		charList[28] = '0' + currentTime % 10;
		charList[27] = '0' + (currentTime / 10) % 10;
		charList[26] = '0' + (currentTime / 100) % 10;
	}
}

void UUi::UpdateScoreUI(int score)
{
	charList[19] = '0' + score % 10;
	charList[18] = '0' + (score / 10) % 10;
	charList[17] = '0' + (score / 100) % 10;
	charList[16] = '0' + (score / 1000) % 10;
	charList[15] = '0' + (score / 10000) % 10;
	charList[14] = '0' + (score / 100000) % 10;
}

void UUi::UpdateCoinUI(int coin)
{
	charList[22] = '0' + coin % 10;
	charList[21] = '0' + (coin / 10) % 10;
}

void UUi::UpdateLifeUI(int life)
{
	charListStart[10] = '0' + life % 10;
}

void UUi::UpdateUV(int index)
{
	UUi::uv = Translate(charList[index]);
}

// char input을 u, v 좌표로 변환 
DirectX::XMFLOAT4 UUi::Translate(char input)
{
	float textureWidth = 1000.0f;
	float textureHeight = 1000.0f;
	float cellWidth = 110.0f;
	float cellHeight = 110.0f;

	float col = 0;
	float row = 0;

	if ('A' <= input && input <= 'I')
	{
		col = 0;
		row = input - 'A';
	}
	else if ('J' <= input && input <= 'R')
	{
		col = 1;
		row = input - 'J';
	}
	else if ('S' <= input && input <= 'Z')
	{
		col = 2;
		row = input - 'S';
	}
	else if ('0' <= input && input <= '8')
	{
		col = 6.1;
		row = input - '0';
	}
	else if (input == '9')
	{
		col = 7.1;
		row = input - '9';
	}
	else if (input == '/')
	{
		col = 8.1;
		row = 1;
	}
	else if (input == '.')
	{
		col = 8.1;
		row = 8;
	}

	float u0 = (cellHeight * row + cellHeight) / textureHeight;
	float v0 = (cellWidth * col + cellWidth) / textureWidth;
	float u1 = cellHeight * row / textureHeight;
	float v1 = cellWidth * col / textureWidth;


	return DirectX::XMFLOAT4(u0, v0, u1, v1);
}