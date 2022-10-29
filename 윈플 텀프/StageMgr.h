#pragma once
#include "stdafx.h"
#include"Stage.h"

#define MAX_STAGE 3

class StageMgr
{
	Stage myStage[3];
public:
	StageMgr();
	Stage& getStage(int index);

private:
	bool isUse[3] = { false };
};
