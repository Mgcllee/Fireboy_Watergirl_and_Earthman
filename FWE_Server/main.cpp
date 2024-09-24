#pragma once

#include "NetworkSettings.h"
#include "GameMaker.h"

int main() {
	NetworkSettings* network_settings = new NetworkSettings(ULONG(INADDR_ANY), USHORT(PORT_NUM));

	SOCKET* listen_socket = network_settings->get_listen_socket();
	GameMaker* game_maker = new GameMaker(listen_socket);
	game_maker->run_game();
	game_maker->cleanup_game();

	network_settings->close_listen_socket();
	return 0;
}