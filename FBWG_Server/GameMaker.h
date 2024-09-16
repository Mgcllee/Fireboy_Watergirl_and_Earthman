#pragma once

#include "StageMaker.h"

class GameMaker : public StageMaker
{
public:
	GameMaker();
	~GameMaker();

	void run_game();
	void cleanup_game();
};

