#pragma once

#include "NetworkSettings.h"
#include "GameMaker.h"

int main() {
	NetworkSettings network_settings(INADDR_ANY, PORT_NUM);

	GameMaker game_maker;
	game_maker.run_game();
	game_maker.cleanup_game();

	return 0;
}