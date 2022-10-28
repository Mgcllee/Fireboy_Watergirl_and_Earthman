#include "StageMgr.h"
#include"Stage.h"

StageMgr::StageMgr()
{
	myStage[0].Stage_1(0);
	myStage[0].stage = 0;
	myStage[1].Stage_1(1);
	myStage[1].stage = 1;
	myStage[2].Stage_1(2);
	isUse[0] = true;
	isUse[1] = true;
}

Stage& StageMgr::getStage(int index)
{
	if (isUse[index])
		return myStage[index];
	else myStage[0];
	// TODO: 여기에 return 문을 삽입합니다.
}

