#include "UGameLogic.h"

UGameLogic& UGameLogic::getGameLogic()
{
	static UGameLogic gameLogic;
	return gameLogic;
}

UGameLogic::UGameLogic()
	: score(0)
{
}

void UGameLogic::setScore(int score)
{
	this->score = score;
}

int UGameLogic::getScore()
{
	return score;
}