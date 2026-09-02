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

POINT charPositionsOver[] =
{
	{ 360, 201 }, { 395, 200 }, { 430, 204 }, { 465, 201 }, 
	{ 525, 203 }, { 560, 205 }, { 595, 200 }, { 630, 202 },

	{ 300, 257 }, { 335, 253 }, { 370, 255 }, { 405, 255 }, { 440, 253 }, { 465, 255 },
	{ 515, 255 }, { 550, 254 }, { 585, 255 }, { 620, 255 }, { 655, 254 }, { 690, 255 },

	{ 220, 801 }, { 255, 801 }, { 290, 800 }, { 325, 804 }, { 360, 804 },
	{ 420, 800 },
	{ 480, 805 }, { 515, 804 },
	{ 570, 802 }, { 605, 800 }, { 640, 805 }, { 675, 805 }, { 710, 800 }, { 745, 804 }, { 780, 805 }
};

char charListOver[] =
{
	'G', 'A', 'M', 'E', 
	'O', 'V', 'E', 'R',

	'S', 'C', 'O', 'R', 'E', ':', '0', '0', '0', '0', '0', '0',

	'P', 'R', 'E', 'S', 'S',
	'A',
	'T', 'O',
	'R', 'E', 'S', 'T', 'A', 'R', 'T'
};

POINT charPositionsStart[] =
{
	{ 470, 494 }, { 505, 490 }, { 540, 490 }, { 575, 490 }, { 610, 490 },
	{ 675, 490 }, { 710, 492 }, { 745, 490 }, { 780, 488 }, { 815, 490 }, { 850, 487 },

	{ 290, 590 },
	{ 355, 590 }, { 390, 590 }, { 425, 588 }, { 460, 592 }, { 495, 588 }, { 530, 590 },
	{ 595, 587 }, { 630, 587 }, { 665, 589 }, { 700, 587 },

	{ 290, 640 },
	{ 355, 640 }, { 390, 640 }, { 425, 638 }, { 460, 642 }, { 495, 638 }, { 530, 640 },
	{ 595, 637 }, { 630, 637 }, { 665, 639 }, { 700, 637 },

	{ 320, 794 }, { 355, 793 }, { 390, 793 }, { 425, 790 },
	{ 490, 790 }, { 525, 790 }, { 560, 790 }, { 595, 790 }, { 630, 790 }, { 665, 790 }
};

char charListStart[] =
{
	'@', '2', '0', '2', '6', 'J', 'U', 'N', 'G', 'L', 'E',

	'1', 'P', 'L', 'A', 'Y', 'E', 'R', 'G', 'A', 'M', 'E',
	'2', 'P', 'L', 'A', 'Y', 'E', 'R', 'G', 'A', 'M', 'E',

	'T', 'O', 'P', ':', '3', '3', '0', '6', '5', '0'
};

//POINT charScoreList [] =
//{
//	{'5', '0'},
//	{'1', '0', '0'},
//	{'2', '0', '0'},
//	{'1', '0', '0', '0'},
//};

//char charScoreList [[]] =
//{
//	{ { -0.01f, 0 }, {0.01f, 0} },
//	{}
//};

UUi::UUi(FVertexUI* UIVertex, DirectX::XMFLOAT2 NDCoord, DirectX::XMFLOAT4 rgba, DirectX::XMFLOAT4 uv)
	:UIVertex(UIVertex)
	, NDCoord(NDCoord)
	, rgba(rgba)
	, uv(uv)
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
		UGameLogic::GetInstance().setGameOver();
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
	charListBlack[9] = '0' + life % 10;
}

void UUi::UpdateFinalScoreUI(int score)
{
	charListEnd[25] = '0' + score % 10;
	charListEnd[24] = '0' + (score / 10) % 10;
	charListEnd[23] = '0' + (score / 100) % 10;
	charListEnd[22] = '0' + (score / 1000) % 10;
	charListEnd[21] = '0' + (score / 10000) % 10;
	charListEnd[20] = '0' + (score / 100000) % 10;
}

void UUi::UpdateOverScoreUI(int score)
{
	charListOver[19] = '0' + score % 10;
	charListOver[18] = '0' + (score / 10) % 10;
	charListOver[17] = '0' + (score / 100) % 10;
	charListOver[16] = '0' + (score / 1000) % 10;
	charListOver[15] = '0' + (score / 10000) % 10;
	charListOver[14] = '0' + (score / 100000) % 10;
}

void UUi::UpdateUV(char* charList, int index)
{
	UUi::uv = UUi::Translate(charList[index]);
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