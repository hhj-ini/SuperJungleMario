#pragma once

class UGameLogic
{
private:
	int score;
	UGameLogic();

public:
	static UGameLogic& getGameLogic();

	void setScore(int score);
	int getScore();
};