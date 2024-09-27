#pragma once

#include "stdafx.h"
#include "object.h"

#define WINDOW_WID			1200
#define WINDOW_HEI			800
#define GROUND_POS_Y		730

#ifndef STAGE_H
#define STAGE_H

class Client;

class Stage
{
public:
	short stage;					
	bool clear;						
	int average;
	bool time_over;					
	int count;						
	int red_total;					
	int blue_total;					
	bool stair;						

	OBJECT Ground{ WINDOW_WID / 2, WINDOW_HEI, WINDOW_WID, WINDOW_HEI - GROUND_POS_Y, 0, 0, true };
	
	queue<OBJECT> jewely;
	vector<OBJECT> Trap;
	vector<OBJECT> Ft;	

	OBJECT Die;		
	OBJECT door;

	int maxJewelyNum = 0;

	array<atomic<char>, 3>* playerRole;
	array<atomic<char>, 3>* selectPlayerRole;

public:
	void title() {}
	void lobby() {}
	void reset_position(int index, array<Client, 3>* positions)
	{
		switch (index)
		{
		case STAGE_01:
			Stage_1(positions);
			break;
		case STAGE_02:
			Stage_2(positions);
			break;
		case STAGE_03:
			Stage_3(positions);
			break;
		default:
			break;
		}
	}
private:
	void Stage_1(array<Client, 3>* positions);
	void Stage_2(array<Client, 3>* positions);
	void Stage_3(array<Client, 3>* positions);
};

#endif