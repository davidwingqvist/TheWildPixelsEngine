#pragma once
class Player
{
private:

	struct Stats
	{
		int hp;
		float speed;
	}stats;

public:

	Player();
	virtual ~Player();

};

