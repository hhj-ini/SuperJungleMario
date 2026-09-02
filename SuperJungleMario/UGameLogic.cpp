#include "UGameLogic.h"

UGameLogic& UGameLogic::GameLogic()
{
	static UGameLogic gameLogic;
	return gameLogic;
}

UGameLogic::UGameLogic()
	: score(0), coin(0), life(3)
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

int UGameLogic::getLife()
{
	return life;
}

void UGameLogic::addOneLife()
{
	life++;
}

void UGameLogic::removeOneLife() // UGameLogic::GameLogic().removeOnelife() 마다 
{
	life--;
	
}