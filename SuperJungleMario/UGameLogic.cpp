#include "UGameLogic.h"

UGameLogic& UGameLogic::GetInstance()
{
	static UGameLogic gameLogic;
	return gameLogic;
}

UGameLogic::UGameLogic()
	: score(0),
	coin(0),
	life(3),
	ending(false),
	started(false),
	gameOver(false),
	x(0.0f),
	y(0.0f),
	IsFloatingScore(false),
	floatingScoreList({}),
	showBlack(false),
	respawn(false),
	needRestart(false),
	restart(false),
	renderUI(true),
	audioReady(true) // 이거 false로 이거 false로이거 false로이거 false로
{
}

int UGameLogic::getScore()
{
	return score;
}
void UGameLogic::addScore(int addAmount, float xCoord, float yCoord) // 해당 객체의 Location.x, y 입력
{
	score = score + addAmount;
	x = xCoord;
	y = yCoord;
	setIsFloatingScore(true);
	floatingScoreList.push_back(addAmount);
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
bool UGameLogic::removeOneLife()
{
	life--;
	if (life < 0)
	{
		setShowBlack(false);
		setGameOver();
	}
	return life > -1;
}

bool UGameLogic::IsEnding()
{
	return ending;
}
void UGameLogic::setEnding()
{
	ending = true;
	setNeedRestart();
}

bool UGameLogic::IsStarted()
{
	return started;
}
void UGameLogic::setStarted()
{
	started = true;
}

bool UGameLogic::bIsFloatingScore()
{
	return IsFloatingScore;
}
void UGameLogic::setIsFloatingScore(bool input)
{
	IsFloatingScore = input;
}

std::vector<int> UGameLogic::getFloatingScoreList()
{
	return floatingScoreList;
}
void UGameLogic::setFloatingScoreList(std::vector<int> &scoreList)
{
	floatingScoreList = scoreList;
}

bool UGameLogic::IsGameOver()
{
	return gameOver;
}
void UGameLogic::setGameOver()
{
	gameOver = true;
	setNeedRestart();
}

bool UGameLogic::IsShowBlack()
{
	return showBlack;
}
void UGameLogic::setShowBlack(bool input)
{
	showBlack = input;
}

bool UGameLogic::IsNeedRestart()
{
	return needRestart;
}
void UGameLogic::setNeedRestart()
{
	needRestart = true;
}

bool UGameLogic::IsRestart()
{
	return restart;
}
void UGameLogic::setRestart(bool input)
{
	restart = input;
}

void UGameLogic::resetAll()
{
	score = 0;
	coin = 0;
	life = 3;
	ending = false;
	started = false;
	gameOver = false;
	x = 0.0f;
	y = 0.0f;
	IsFloatingScore = false;
	showBlack = false;
	respawn = false;
	needRestart = false;
	restart = false;
	renderUI = true;
}

bool UGameLogic::IsRenderUI()
{
	return renderUI;
}
void UGameLogic::setRenderUI(bool input) 
{
	renderUI = input;
}

bool UGameLogic::IsAudioReady()
{
	return audioReady;
}
void UGameLogic::setAudioReady()
{
	audioReady = true;
}

DirectX::XMFLOAT2 UGameLogic::getCoordinate()
{
	return DirectX::XMFLOAT2(x, y);
}