#include "UGameLogic.h"

UGameLogic& UGameLogic::GetInstance()
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

void UGameLogic::removeOneLife() // 이게 호출이 되면, UI에서 검은 화면 + 마리오 원점으로 
{
	life--;
	
}