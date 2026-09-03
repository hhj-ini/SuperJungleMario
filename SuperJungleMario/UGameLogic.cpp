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
	audioReady(true), // 이거 false로 이거 false로이거 false로이거 false로
	stoppedTime(0.0f)
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
	soundManager->StopSound(SoundBufferMap[L"BGM"]);

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
	if (!ending)
	{
		soundManager->StopSound(SoundBufferMap[L"BGM"]);
		soundManager->StopSound(SoundBufferMap[L"GameOver"]);
		soundManager->PlaySoundResource(SoundBufferMap[L"StageClear"]);
	}
	ending = true;
	setNeedRestart();
}

bool UGameLogic::IsStarted()
{
	return started;
}
void UGameLogic::setStarted()
{	// 완전히 새로운 판(타이틀에서 게임 시작을 설정하는 함수)
	started = true;
	//soundManager->PlaySoundResource(SoundBufferMap[L"BGM"]);
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
	if (!gameOver)	// 처음 한번만 실행되게
	{
		soundManager->StopSound(SoundBufferMap[L"BGM"]);

		soundManager->PlaySoundResource(SoundBufferMap[L"GameOver"]);
	}
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
	if (false == input)
	{
		soundManager->PlaySoundResource(SoundBufferMap[L"BGM"]);
	}
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
	stoppedTime = 0.0f;
	stoppedTimeSwitch = false;
	soundManager->StopSound(SoundBufferMap[L"GameOver"]);
	//soundManager->PlaySoundResource(SoundBufferMap[L"BGM"]);
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

double UGameLogic::getStoppedTime()
{
	return stoppedTime;
}
void UGameLogic::addStoppedTime(double time)
{
	if (!stoppedTimeSwitch)
	{
		stoppedTimeSwitch = true;
		return;
	}
	stoppedTime += time;
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
