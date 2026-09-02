#pragma once

class UGameLogic
{
public: // 나중에 privite으로 
	int score;
	int coin;
	int life;
	bool ending;
	bool started;
	UGameLogic();

public:
	static UGameLogic& GetInstance();

	int getScore();
	void addScore(int addAmount);

	int getCoin();
	void addOneCoin();

	int getLife();
	void addOneLife();
	void removeOneLife();

	bool IsEnding();
	void setEnding();

	bool IsStarted();
	void setStarted();
};