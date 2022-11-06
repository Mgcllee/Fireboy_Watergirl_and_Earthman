#pragma once
#include "stdafx.h"
#include"Stage.h"

#define MAX_STAGE 5

class StageMgr
{
	//구현 스테이지
	//1. start버튼 스테이지
	//2. 로딩 스테이지
	//3. 스테이지1
	//4. 스테이지2
	//5. 스테이지3

	//총 5가지
	Stage myStage[MAX_STAGE];
public:
	StageMgr();
	Stage& getStage(int index);

private:
	bool isUse[MAX_STAGE] = { false };
};
