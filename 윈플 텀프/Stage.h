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

	float g = 4;			// 중력 조절로 점프 높이 조정
	float v = 0;			// 이동 속도
	bool Down = FALSE;
};

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
	std::queue<OBJECT> jewely;

	std::vector<OBJECT> Trap;
	std::vector<OBJECT> Ft;			// 발판 오브젝트

	// OBJECT Trap[90];		
	OBJECT Die;				// 사망시 나오는 연기 (Max x = 7950, move x = 159)
	OBJECT door;		// (Max x = 1260, move x = 60) // Max = 1296, Move = 54
	OBJECT button;
	OBJECT block;
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

	void Push();			// 캐릭터가 "박스 오브젝트" 밀기 함수 (Fire boy와 Water girl이 동시에 양쪽에서 밀 경우 처리)
	void Jump();			// 캐릭터 점프
	void Foot();			// 캐릭터가 발판에 올라갔을 때, 충돌처리
};

