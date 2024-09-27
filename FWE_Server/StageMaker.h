#pragma once

#include "stdafx.h"
#include "Timer.h"
#include "Stage.h"
#include "ClientAccepter.h"
#include "StagePosition.h"

class StageMaker
{
public:
	StageMaker();

	void run_game_stage_thread(array<Client, 3>* game_member, Stage* game_stage);
	void reset_stage_timer();

	void check_all_client_role();
	void show_game_stage(int stage_number);
	
	bool check_door();
	bool check_jewely();


	bool check_next_stage_condition();
	void update_stage_index();
	bool check_retry_game_condition();

	bool show_player_score();

private:
	int stage_index;

	mutex select_mutex;

	int currentJewelyNum = 0;
	bool isVisibleDoor = false;
	mutex jewelyMutex;

	Timer* timer;

	array<Client, 3>* clients;

	Stage* stage_item;
};
