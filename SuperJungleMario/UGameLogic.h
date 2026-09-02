#pragma once
#include <DirectXMath.h>

class UGameLogic
{
public: // 나중에 privite으로 
	int score;
	int coin;
	int life;
	bool ending;
	bool started;
	bool gameOver;
	float x;
	float y;
	bool showScore;
	bool showBlack;
	bool respawn;
	bool needRestart;
	bool restart;
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

	bool IsShowScore();
	void setShowScore(bool input);

	bool IsGameOver();
	void setGameOver();

	bool IsShowBlack();
	void setShowBlack(bool input);

	bool IsNeedRestart();
	void setNeedRestart();

	bool IsRestart();
	void setRestart(bool input);

	void resetAll();

	DirectX::XMFLOAT2 getCoordinate();
};