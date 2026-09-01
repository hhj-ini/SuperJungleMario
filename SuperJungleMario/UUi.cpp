#include "UUi.h"

POINT charPositions[] =
{
	{ 84, 49 },
	{ 119, 45 },
	{ 158, 49 },
	{ 188, 45 },
	{ 227, 49 },
	{ 613, 47 },
	{ 648, 45 },
	{ 683, 45 },
	{ 712, 45 },
	{ 747, 42 },
	{ 846, 47 },
	{ 871, 42 },
	{ 905, 45 },
	{ 940, 42 },
	{ 84, 84 },
	{ 119, 84 },
	{ 155, 84 },
	{ 190, 84 },
	{ 226, 84 },
	{ 261, 84 },
	{ 435, 84 },
	{ 475, 84 },
	{ 510, 84 },
	{ 638, 84 },
	{ 683, 84 },
	{ 724, 84 },
	{ 871, 84 },
	{ 905, 84 },
	{ 940, 84 }
};

char charList[] =
{
	'M', 'A', 'R', 'I', 'O',
	'W', 'O', 'R', 'L', 'D',
	'T', 'I', 'M', 'E',
	'0', '0', '0', '3', '0', '0',
	'x', '0', '1',
	'1', '-', '1',
	'0', '0', '0'
};

UUi::UUi(FVertexUI* UIVertex, DirectX::XMFLOAT2 NDCoord, DirectX::XMFLOAT4 rgba, DirectX::XMFLOAT4 uv, float scale)
	:UIVertex(UIVertex)
	,NDCoord(NDCoord)
	,rgba(rgba)
	,uv(uv)
	,scale(scale)
{
}

void UUi::Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight)
{
	renderer.UpdateUI(UUi::NDCoord, vertexBuffer, UIWidth, UIHeight, UUi::uv, UUi::rgba);
	renderer.RenderUI(vertexBuffer, numVertices);
}

void UUi::setNDCoord(DirectX::XMFLOAT2 NDCoord)
{
	UUi::NDCoord = NDCoord;
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

	float u0 = (cellHeight * row + cellHeight) / textureHeight;
	float v0 = (cellWidth * col + cellWidth) / textureWidth;
	float u1 = cellHeight * row / textureHeight;
	float v1 = cellWidth * col / textureWidth;


	return DirectX::XMFLOAT4(u0, v0, u1, v1);
}