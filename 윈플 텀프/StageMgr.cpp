#include "StageMgr.h"
#include"Stage.h"

StageMgr::StageMgr()
{
	myStage[0].title();		// start Title 스테이지
	myStage[0].stage = STAGE_TITLE;

	myStage[1].Loading();	// 로딩 스테이지
	myStage[1].stage = STAGE_LOADING;

	myStage[2].lobby();		// lobby 스테이지 (=캐릭터 선택 스테이지)
	myStage[2].stage = STAGE_ROLE;

	myStage[3].Stage_1();
	myStage[3].stage = STAGE_01;

	myStage[4].Stage_2();
	myStage[4].stage = STAGE_02;

	myStage[5].Stage_3();
	myStage[5].stage = STAGE_03;

	// 아직 구현된 부분만 true
	isUse[STAGE_TITLE]	= true;
	isUse[STAGE_LOADING]= true;
	isUse[STAGE_ROLE]	= true;
	isUse[STAGE_01]		= true;
	isUse[STAGE_02]		= false;
	isUse[STAGE_03]		= false;
}

Stage& StageMgr::getStage(int index)
{
	if (isUse[index])
		return myStage[index];
	else myStage[0];
	// TODO: 여기에 return 문을 삽입합니다.
}

