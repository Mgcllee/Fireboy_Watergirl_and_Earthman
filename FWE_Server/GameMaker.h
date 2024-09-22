#pragma once

#include "StageMaker.h"
#include "Client.h"

class GameMaker : public StageMaker {
public:
	void run_game();
	void create_game_threads();
	void join_game_threads();
	
private:
	thread stage_maker_thread;
	array<thread, 3> client_threads;
};