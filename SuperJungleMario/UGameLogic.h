#pragma once
#include <DirectXMath.h>
#include <vector>

class UGameLogic
{
private: // 나중에 privite으로 
	int score;
	int coin;
	int life;
	bool ending;
	bool started;
	bool gameOver;
	float x;
	float y;
	bool IsFloatingScore;
	int lastScore;
	bool showBlack;
	bool respawn;
	bool needRestart;
	bool restart;
	bool renderUI;
	bool audioReady;
	UGameLogic();

public:
	static UGameLogic& GetInstance();

	int getScore();
	void addScore(int addAmount, float x, float y);

	int getCoin();
	void addOneCoin();

	int getLife();
	void addOneLife();
	bool removeOneLife();

	bool IsEnding();
	void setEnding();

	bool IsStarted();
	void setStarted();

	bool bIsFloatingScore();
	void setIsFloatingScore(bool input);

	int getLastScore();
	void setLastScore(int score);

	bool IsGameOver();
	void setGameOver();

	bool IsShowBlack();
	void setShowBlack(bool input);

	bool IsNeedRestart();
	void setNeedRestart();

	bool IsRestart();
	void setRestart(bool input);

	void resetAll();

	bool IsRenderUI();
	void setRenderUI(bool input);

	bool IsAudioReady();
	void setAudioReady();

	DirectX::XMFLOAT2 getCoordinate();
};