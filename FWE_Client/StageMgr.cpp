#include"stdafx.h"	
#include "StageMgr.h"
#include"Stage.h"


int StageMgr::StageTimepass = 0;
int StageMgr::EndStageTime = 50;
bool StageMgr::IsTimeoutStageEnd = false;

StageMgr::StageMgr()
{
	myStage[0].title();		
	myStage[0].stage = STAGE_TITLE;

	myStage[1].Loading();	
	myStage[1].stage = STAGE_LOADING;

	myStage[2].lobby();		
	myStage[2].stage = STAGE_ROLE;

	myStage[STAGE_01].Stage_1();
	myStage[STAGE_01].stage = STAGE_01;

	myStage[STAGE_02].Stage_2();
	myStage[STAGE_02].stage = STAGE_02;

	myStage[STAGE_03].Stage_3();
	myStage[STAGE_03].stage = STAGE_03;

	myStage[RESULT];
	myStage[RESULT].stage = RESULT;

	isUse[STAGE_TITLE]	= true;
	isUse[STAGE_LOADING]= true;
	isUse[STAGE_ROLE]	= true;
	isUse[STAGE_01]		= true;
	isUse[STAGE_02]		= true;
	isUse[STAGE_03]		= true;
	isUse[RESULT]		= true;
}

void StageMgr::ResetStage()
{
	StageMgr::StageTimepass = 0;
	StageMgr::IsTimeoutStageEnd = false;
}


Stage& StageMgr::getStage(int index)
{
	if (isUse[index])
		return myStage[index];
	return myStage[0];
}

