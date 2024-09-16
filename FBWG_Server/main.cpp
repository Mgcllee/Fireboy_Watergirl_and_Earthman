#pragma once

#include "NetworkSettings.h"
#include "StageMaker.h"

int main()
{
	NetworkSettings* network_settings
		= new NetworkSettings(INADDR_ANY, PORT_NUM);

	thread stage_maker = thread(
		StageMaker::make_game_stage, &StageMaker());
	stage_maker.join();

	WSACleanup();
	return 0;
}