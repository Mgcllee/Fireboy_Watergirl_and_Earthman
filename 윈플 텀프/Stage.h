#pragma once
#include "stdafx.h"
//#include "ImageMgr.h"

class Stage
{
public:
	RECTANGLE Rt;

	short stage;						// 현재 스테이지
	BOOL clear;							// Stage 클리어 여부
	int average;						
	BOOL time_over;						// 현재 Stage에서 Timer Over 여부
	int count;							// 애니메이션 프레임 번호
	int red_total;						// 빨강 보석 총 개수
	int blue_total;						// 파랑 보석 총 개수
	BOOL stair;							// Fire boy와 Water girl 이 문 안 계단을 올라감

	// 	OBJECT(int pos_x, int pos_y, int WID, int HEI, int MAXWID, int IMAGEMOVEWID, bool ON)
	OBJECT Ground{0, GROUND_POS_Y, WINDOW_WID, WINDOW_HEI - GROUND_POS_Y, 0, 0, TRUE};
	std::vector<OBJECT> Red_Jewel;
	std::vector<OBJECT> Blue_Jewel;
	std::vector<OBJECT> Trap;
	std::vector<OBJECT> Ft;			// 발판 오브젝트

	// OBJECT Trap[90];		
	OBJECT Die;				// 사망시 나오는 연기 (Max x = 7950, move x = 159)
	OBJECT blue_door;		// (Max x = 1260, move x = 60) // Max = 1296, Move = 54
	OBJECT red_door;		// (Max x = 1260, move x = 60) // Max = 1250, Move = 50
	OBJECT button[5];		// 노랑 버튼(block1.PNG 참고)	(Down BTN Max y = 7, move y = 1) else (Up BTN Max y = 15, move y = 1)
	OBJECT block[5];		// (Max x = 40, move x = 2)

public:
	void title() {}
	void lobby() {}
	void Loading();
	void SelectCharacter();
	void Stage_1();
	void Stage_2();
	void Stage_3();

	void Push();			// 캐릭터가 "박스 오브젝트" 밀기 함수 (Fire boy와 Water girl이 동시에 양쪽에서 밀 경우 처리)
	void Jump();			// 캐릭터 점프
	void Foot();			// 캐릭터가 발판에 올라갔을 때, 충돌처리
};

extern Stage currentStage;
