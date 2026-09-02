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

POINT charPositionsBlack[] =
{
	{ 355, 352 }, { 390, 350 }, { 425, 350 }, { 454, 349 }, { 489, 346 },
	{ 570, 348 }, { 610, 350 }, { 643, 347 },
	{ 519, 505 }, { 614, 500 },
};

char charListBlack[] =
{
	'W', 'O', 'R', 'L', 'D',
	'1', '/', '1',
	'X', '3',
};

POINT charPositionsEnd[] =
{
	{ 250, 205 }, { 285, 200 }, { 320, 200 }, { 355, 203 }, { 390, 203 },
	{ 450, 205 }, { 485, 204 }, { 520, 205 },
	{ 580, 203 }, { 615, 200 }, { 650, 203 }, { 685, 200 }, { 720, 203 }, { 755, 203 },

	{ 300, 257 }, { 335, 253 }, { 370, 255 }, { 405, 255 }, { 440, 253 }, { 465, 255 },
	{ 515, 255 }, { 550, 254 }, { 585, 255 }, { 620, 255 }, { 655, 254 }, { 690, 255 },

	{ 390, 450 }, { 425, 452 }, { 460, 450 }, { 495, 450 }, { 530, 451 }, { 565, 455 }, { 600, 455 },
	{ 240, 552 }, { 275, 550 }, { 310, 554 },
	{ 380, 552 }, { 415, 553 }, { 450, 551 },
	{ 520, 551 }, { 555, 551 }, { 590, 550 },
	{ 660, 550 }, { 695, 550 }, { 730, 554 },

	{ 220, 801 }, { 255, 801 }, { 290, 800 }, { 325, 804 }, { 360, 804 },
	{ 420, 800 },
	{ 480, 805 }, { 515, 804 },
	{ 570, 802 }, { 605, 800 }, { 640, 805 }, { 675, 805 }, { 710, 800 }, { 745, 804 }, { 780, 805 }
};

char charListEnd[] =
{
	'T', 'H', 'A', 'N', 'K', 
	'Y', 'O', 'U', 
	'M', 'A', 'R', 'I', 'O', '!',

	'S', 'C', 'O', 'R', 'E', ':', '0', '0', '0', '0', '0', '0',

	'C', 'R', 'E', 'D', 'I', 'T', 'S', 
	'K', 'H', 'W', 'K', 'T', 'H', 'L', 'K', 'H', 'H', 'H', 'J',

	'P', 'R', 'E', 'S', 'S',
	'A',
	'T', 'O',
	'R', 'E', 'S', 'T', 'A', 'R', 'T'
};

POINT charPositionsStart[] =
{
	{ 470, 494 }, { 505, 490 }, { 540, 490 }, { 575, 490 }, { 610, 490 },
	{ 675, 490 }, { 710, 492 }, { 745, 490 }, { 780, 488 }, { 815, 490 }, { 850, 487 },

	{ 370, 590 },
	{ 435, 590 }, { 470, 590 }, { 505, 588 }, { 540, 592 }, { 575, 588 }, { 610, 590 },
	{ 675, 587 }, { 710, 587 }, { 745, 589 }, { 780, 587 },

	{ 370, 640 },
	{ 435, 640 }, { 470, 640 }, { 505, 638 }, { 540, 642 }, { 575, 638 }, { 610, 640 },
	{ 675, 637 }, { 710, 637 }, { 745, 639 }, { 780, 637 },

	{ 430, 794 }, { 465, 793 }, { 500, 793 }, { 535, 790 }, 
	{ 600, 790 }, { 635, 790 }, { 670, 790 }, { 705, 790 }, { 740, 790 }, { 775, 790 }
};

char charListStart[] =
{
	'@', '2', '0', '2', '6', 'J', 'U', 'N', 'G', 'L', 'E',

	'1', 'P', 'L', 'A', 'Y', 'E', 'R', 'G', 'A', 'M', 'E',
	'2', 'P', 'L', 'A', 'Y', 'E', 'R', 'G', 'A', 'M', 'E',

	'T', 'O', 'P', ':', '3', '3', '0', '6', '5', '0'
};

UUi::UUi(FVertexUI* UIVertex, DirectX::XMFLOAT2 NDCoord, DirectX::XMFLOAT4 rgba, DirectX::XMFLOAT4 uv, float scale)
	:UIVertex(UIVertex)
	, NDCoord(NDCoord)
	, rgba(rgba)
	, uv(uv)
	, scale(scale)
{
}

void UUi::Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight, DirectX::XMFLOAT2 NDCoord)
{
	UUi::setNDCoord(NDCoord);
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
	charListBlack[10] = '0' + life % 10;
}

void UUi::UpdateFinalScoreUI(int score)
{
	charList[25] = '0' + score % 10;
	charList[24] = '0' + (score / 10) % 10;
	charList[23] = '0' + (score / 100) % 10;
	charList[22] = '0' + (score / 1000) % 10;
	charList[21] = '0' + (score / 10000) % 10;
	charList[20] = '0' + (score / 100000) % 10;
}

void UUi::UpdateUV(int index)
{
	UUi::uv = Translate(charList[index]);
}

void UUi::UpdateUVBlack(int index)
{
	UUi::uv = Translate(charListBlack[index]);
}

void UUi::UpdateUVEnd(int index)
{
	UUi::uv = Translate(charListEnd[index]);
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
	else if (input == '!')
	{
		col = 7.1;
		row = 8;
	}
	else if (input == '@')
	{
		col = 8.1;
		row = 7;
	}
	else if (input == ':')
	{
		col = 7.1;
		row = 4;
	}

	float u0 = (cellHeight * row + cellHeight) / textureHeight;
	float v0 = (cellWidth * col + cellWidth) / textureWidth;
	float u1 = cellHeight * row / textureHeight;
	float v1 = cellWidth * col / textureWidth;


	return DirectX::XMFLOAT4(u0, v0, u1, v1);
}