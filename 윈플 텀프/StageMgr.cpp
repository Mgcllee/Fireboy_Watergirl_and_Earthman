#include "StageMgr.h"
#include"Stage.h"

StageMgr::StageMgr()
{
	myStage[0].Stage_0();
	myStage[0].stage = 0;

	myStage[1].Stage_1();
	myStage[1].stage = 1;
	
	myStage[2].Stage_2();
	myStage[2].stage = 1;

	myStage[3].Stage_3();
	myStage[3].stage = 3;

	isUse[0] = true;
	isUse[1] = true;
	isUse[2] = true;
	isUse[3] = false;
}

Stage& StageMgr::getStage(int index)
{
	if (isUse[index])
		return myStage[index];
	else myStage[0];
	// TODO: 여기에 return 문을 삽입합니다.
}

