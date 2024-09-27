#pragma once

#include "GameMaker.h"


void GameMaker::run_game() {
	game_stage.playerRole = &playerRole;
	game_stage.selectPlayerRole = &selectPlayerRole;

	create_game_threads();
	join_game_threads();
}

void GameMaker::create_game_threads() {
	stage_maker_thread = thread(&StageMaker::run_game_stage_thread, 
		new StageMaker, &clients, &game_stage);

	for (int user_ticket = 0; user_ticket < 3; ++user_ticket) {
		client_threads[user_ticket] = thread(&Client::run_client_thread,
			new Client, &clients, &game_stage, user_ticket);
	}
}

void GameMaker::join_game_threads() {
	stage_maker_thread.join();

	for (thread& client_thread : client_threads) {
		client_thread.join();
	}
}

void GameMaker::cleanup_game() {
	for (Client& client : clients) {
		if (client.network_socket != INVALID_SOCKET) {
			closesocket(client.network_socket);
			client.network_socket = INVALID_SOCKET;
		}
	}
	WSACleanup();
}

array<Client, 3>* GameMaker::get_clients() {
	return &clients;
}
