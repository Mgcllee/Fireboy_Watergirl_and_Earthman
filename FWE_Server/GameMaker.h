#pragma once

#include "StageMaker.h"
#include "Client.h"

class GameMaker : public StageMaker {
public:
	void run_game();
	void create_game_threads();
	void join_game_threads();
	
	array<Client, 3>* get_clients();

private:
	thread stage_maker_thread;
	array<thread, 3> client_threads;

	Stage game_stage;
	array<Client, 3> clients;

	array<atomic<char>, 3> playerRole = { 'f', 'f', 'f' };
	array<atomic<char>, 3> selectPlayerRole = { 'n', 'n', 'n' };
};