#pragma once

#include "StageMaker.h"
#include "Client.h"

class GameMaker : public StageMaker {
public:
	GameMaker(SOCKET* server_socket);

	void run_game();
	void create_game_threads();
	void join_game_threads();
	
private:
	SOCKET* listen_socket;

	thread stage_maker_thread;
	array<thread, 3> client_threads;

	Stage game_stage;
	array<Client, 3> clients;
};