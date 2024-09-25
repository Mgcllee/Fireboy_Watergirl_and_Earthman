#pragma once

#include "NetworkSettings.h"
#include "GameMaker.h"

int main() {
	NetworkSettings* network_settings = new NetworkSettings(ULONG(INADDR_ANY), USHORT(PORT_NUM));

	GameMaker* game_maker = new GameMaker();
	array<Client, 3>* clients = game_maker->get_clients();

	SOCKET* listen_socket = network_settings->get_listen_socket();
	ClientAccepter client_accepter(listen_socket);
	while (false == client_accepter.accept_all_client(clients)) {
		network_settings->reset_listen_socket();
	}

	game_maker->run_game();
	game_maker->cleanup_game();

	network_settings->close_listen_socket();
	return 0;
}