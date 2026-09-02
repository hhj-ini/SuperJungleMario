#include "UGameLogic.h"

UGameLogic& UGameLogic::GetInstance()
{
	static UGameLogic gameLogic;
	return gameLogic;
}

UGameLogic::UGameLogic()
	: score(0), coin(0), life(3), ending(false), started(true), x(0.0f), y(0.0f), showScore(false)
{
}

int UGameLogic::getScore()
{
	return score;
}
void UGameLogic::addScore(int addAmount, float xCoord, float yCoord)
{
	score = score + addAmount;
	x = xCoord;
	y = yCoord;
	setShowScore(true);
}

int UGameLogic::getCoin()
{
	return coin;
}
void UGameLogic::addOneCoin()
{
	coin++;
	if (coin >= 100)
	{
		addOneLife();
		coin = 0;
	}
}

int UGameLogic::getLife()
{
	return life;
}
void UGameLogic::addOneLife()
{
	life++;
}
void UGameLogic::removeOneLife()
{
	life--;
}

bool UGameLogic::IsEnding()
{
	return ending;
}
void UGameLogic::setEnding()
{
	ending = true;
}

bool UGameLogic::IsStarted()
{
	return started;
}
void UGameLogic::setStarted()
{
	started = true;
}

bool UGameLogic::IsShowScore()
{
	return showScore;
}
void UGameLogic::setShowScore(bool input)
{
	showScore = input;
}

DirectX::XMFLOAT2 UGameLogic::getCoordinate()
{
	return DirectX::XMFLOAT2(x, y);
}