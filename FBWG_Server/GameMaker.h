#pragma once

#include "StageMaker.h"
#include "Client.h"

class GameMaker : public StageMaker
{
public:
	GameMaker();
	~GameMaker();

	void run_game();
	void create_game_threads();
	void reset_game();
	void start_game();
	void join_game_threads();
	
private:
	thread stage_maker_thread;
	array<thread, 3> client_threads;
};

