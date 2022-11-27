#pragma once
#include "stdafx.h"
#include"Stage.h"

#define MAX_STAGE 6

class StageMgr
{
	//[구현 스테이지]
	//1. start버튼 스테이지
	//2. 로비 스테이지
	//3. 로딩 스테이지
	//4. 스테이지1
	//5. 스테이지2
	//6. 스테이지3

	//총 5가지
	Stage myStage[MAX_STAGE];
public:
	StageMgr();
	Stage& getStage(int index);

	static int StageTimepass;
	static int EndStageTime;
	static bool IsTimeoutStageEnd;

	void ResetStage();
private:
	bool isUse[MAX_STAGE] = { false };
};
