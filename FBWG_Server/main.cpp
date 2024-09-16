#pragma once

#include "NetworkSettings.h"
#include "StageMaker.h"

int main() {
	NetworkSettings network_settings(INADDR_ANY, PORT_NUM);

	StageMaker game_maker;
	game_maker.run_game();
	game_maker.cleanup_game();

	return 0;
}