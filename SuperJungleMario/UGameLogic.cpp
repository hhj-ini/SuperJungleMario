#include "UGameLogic.h"

UGameLogic& UGameLogic::GameLogic()
{
	static UGameLogic gameLogic;
	return gameLogic;
}

UGameLogic::UGameLogic()
	: score(0), coin(0)
{
}

int UGameLogic::getScore()
{
	return score;
}

void UGameLogic::addScore(int addAmount)
{
	this->score = this->score + addAmount;
}

int UGameLogic::getCoin()
{
	return coin;
}

void UGameLogic::addOneCoin()
{
	coin++;
}