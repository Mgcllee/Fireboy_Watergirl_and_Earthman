#pragma once

#include "stdafx.h"
#include "Timer.h"
#include "Stage.h"
#include "ClientAccepter.h"

class StageMaker
{
public:
	StageMaker();
	~StageMaker();

	void make_game_stage();

	void cleanup_game();
private:
	int stageIndex = -1;

	Stage StageMgr;

	int currentJewelyNum = 0;
	bool isVisibleDoor = false;
	mutex jewelyMutex;

	Timer _timer;
	bool isTimeOut = false;
	bool gameEnd = false;
	double timeoutSeconds = 50;


	mutex selectMutex;
	array<char, 3> playerRole = { 'f', 'f', 'f' };
	array<char, 3> selectPlayerRole = { 'n', 'n', 'n' };

	array<Client, 3> clients;

};

