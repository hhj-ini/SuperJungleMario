#pragma once

class UGameLogic
{
public: // 나중에 privite으로 
	int score;
	int coin;
	int life;
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
};