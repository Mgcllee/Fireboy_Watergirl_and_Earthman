#pragma once

#include "GameMaker.h"

GameMaker::GameMaker(SOCKET* server_socket)
	: StageMaker(server_socket)
	, listen_socket(server_socket)
{

}

void GameMaker::run_game() {
	while (true) {
		create_game_threads();
		join_game_threads();
	}
}

void GameMaker::create_game_threads() {
	stage_maker_thread = thread(&StageMaker::run_game_stage_thread, 
		new StageMaker(listen_socket), &clients, &game_stage);

	for (thread& client_thread : client_threads) {
		client_thread = thread(&Client::run_client_thread, 
			new Client, &clients, &game_stage);
	}
}

void GameMaker::join_game_threads() {
	for (thread& client_thread : client_threads) {
		client_thread.join();
	}

	stage_maker_thread.join();
}