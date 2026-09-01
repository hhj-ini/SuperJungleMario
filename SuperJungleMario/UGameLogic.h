#pragma once

class UGameLogic
{
public: // 나중에 privite으로 
	int score;
	int coin;
	UGameLogic();

public:
	static UGameLogic& GameLogic();

	int getScore();
	void addScore(int addAmount);
	int getCoin();
	void addOneCoin();
};