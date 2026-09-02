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
	showScore(false),
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
{
	started = true;
	soundManager->PlaySoundResource(SoundBufferMap[L"BGM"]);
}

bool UGameLogic::IsShowScore()
{
	return showScore;
}
void UGameLogic::setShowScore(bool input)
{
	showScore = input;
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
	showScore = false;
	showBlack = false;
	respawn = false;
	needRestart = false;
	restart = false;
	renderUI = true;
	soundManager->StopSound(SoundBufferMap[L"GameOver"]);
	soundManager->PlaySoundResource(SoundBufferMap[L"BGM"]);
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
