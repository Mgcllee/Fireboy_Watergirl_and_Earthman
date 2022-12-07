#pragma once
//#include "stdafx.h"
//#include "ImageMgr.h"
#include<queue>
#include"Object.h"

#define WINDOW_WID			1200
#define WINDOW_HEI			800
#define GROUND_POS_Y		730

struct RECTANGLE {
	int x;
	int y, dic = 0;

	float g = 4;		
	float v = 0;		
	bool Down = FALSE;
};

class Stage
{
public:
	RECTANGLE Rt;

	short stage;		
	BOOL clear;			
	int average;		
	BOOL time_over;		
	int count;			
	int red_total;		
	int blue_total;		
	BOOL stair;			


	OBJECT Ground{0, GROUND_POS_Y, WINDOW_WID, WINDOW_HEI - GROUND_POS_Y, 0, 0, TRUE};
	std::queue<OBJECT> jewely;

	std::vector<OBJECT> Trap;
	std::vector<OBJECT> Ft;			// 발판 오브젝트

	OBJECT Die;				
	OBJECT door;		
	OBJECT currentVisibleJewely;

	int maxJewelyNum = 0;


public:
	void title() {}
	void lobby() {}
	void Loading();
	void SelectCharacter();
	void Stage_1();
	void Stage_2();
	void Stage_3();

	void Push();			
	void Jump();			
	void Foot();			
};

