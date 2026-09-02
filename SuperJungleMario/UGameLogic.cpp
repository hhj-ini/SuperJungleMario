#include "UGameLogic.h"

UGameLogic& UGameLogic::GetInstance()
{
	static UGameLogic gameLogic;
	return gameLogic;
}

UGameLogic::UGameLogic()
	: score(0), coin(0), life(3), ending(false), started(true)
{
}

int UGameLogic::getScore()
{
	return score;
}
void UGameLogic::addScore(int addAmount)
{
	score = score + addAmount;
}

int UGameLogic::getCoin()
{
	return coin;
}
void UGameLogic::addOneCoin()
{
	coin++;
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