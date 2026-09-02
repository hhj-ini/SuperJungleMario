#include "UGameLogic.h"
#include "ResourceManager.h"
#include "USoundManager.h"

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
	lastScore(0),
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
	setLastScore(addAmount);
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
	soundManager->PlaySoundResource(SoundBufferMap[L"GameOver"]);
}

bool UGameLogic::bIsFloatingScore()
{
	return IsFloatingScore;
}
void UGameLogic::setIsFloatingScore(bool input)
{
	IsFloatingScore = input;
}

int UGameLogic::getLastScore()
{
	return lastScore;
}
void UGameLogic::setLastScore(int score)
{
	lastScore = score;
}

bool UGameLogic::IsGameOver()
{
	return gameOver;
}
void UGameLogic::setGameOver()
{
	gameOver = true;
	soundManager->StopSound(SoundBufferMap[L"BGM"]);

	soundManager->PlaySoundResource(SoundBufferMap[L"GameOver"]);
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

void UGameLogic::setSoundManager(USoundManager* InSoundManager)
{
	soundManager = InSoundManager;

	std::wstring soundName = L"BGM";	//설정한 이름으로 접근 가능
	SoundBufferMap[soundName] = ResourceManager::GetInstance().GetSoundResource(L"Resource\\Sound\\GroundTheme.wav", soundManager);

	soundName = L"GameOver";	//설정한 이름으로 접근 가능
	SoundBufferMap[soundName] = ResourceManager::GetInstance().GetSoundResource(L"Resource\\Sound\\GameoverBgm.wav", soundManager);

	soundName = L"StageClear";	//설정한 이름으로 접근 가능
	SoundBufferMap[soundName] = ResourceManager::GetInstance().GetSoundResource(L"Resource\\Sound\\StageClear.wav", soundManager);
}
