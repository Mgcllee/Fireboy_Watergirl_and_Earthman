#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include "object.h"

#define WINDOW_WID			1200
#define WINDOW_HEI			800
#define GROUND_POS_Y		730

class PLAYER {
public:
	int id;						// 다른 클라이언트 식별 정보 - -1 // 아직 안쓰는 상황에 -1
	short role;					// 어떤 캐릭터 인지 - f w e => fire water earth
	int x, y;					// 우하단 좌표
	int hei = 100, wid = 60;	// 캐릭터 크기

	int direction;				// 이동방향 (애니메이션에서 사용)
	float wid_v{};
	float wid_a{};

	int ground = 730;		// Stage의 바닥 높이 (상단이 y = 0, 내려갈수록 +)
	float g = 3.f;			// 중력 조절로 점프 높이 조정
	float v = 0;			// 속도
	short Frame = 0;		// 애니메이션 프레임

	int C_img_Frame{};
	int C_img_x{}, C_img_y{};
	int C_img_X_Size_01{}, C_img_Y_Size_01{};
	int C_img_X_Size_02{}, C_img_Y_Size_02{};

	bool on = true;
	bool is_Jumping = true;
	bool is_Push = true;
	bool Down = true;

	PLAYER() : id(-1), role('f') {}
	~PLAYER() {

	}
};

class Stage
{
public:
	short stage;						// 현재 스테이지
	bool clear;							// Stage 클리어 여부
	int average;
	bool time_over;						// 현재 Stage에서 Timer Over 여부
	int count;							// 애니메이션 프레임 번호
	int red_total;						// 빨강 보석 총 개수
	int blue_total;						// 파랑 보석 총 개수
	bool stair;							// Fire boy와 Water girl 이 문 안 계단을 올라감

	OBJECT Ground{ WINDOW_WID / 2, WINDOW_HEI, WINDOW_WID, WINDOW_HEI - GROUND_POS_Y, 0, 0, true };
	
	std::queue<OBJECT> jewely;
	std::vector<OBJECT> Trap;
	std::vector<OBJECT> Ft;	

	OBJECT Die;		
	OBJECT door;	
	OBJECT currentVisibleJewely;

	int maxJewelyNum = 0;

public:
	void title() {}
	void lobby() {}
	void getStage(int index)
	{
		switch (index)
		{
		case STAGE_01:
			Stage_1();
			break;
		case STAGE_02:
			Stage_2();
			break;
		case STAGE_03:
			Stage_3();
			break;
		default:
			break;
		}
	}
private:
	void Stage_1();
	void Stage_2();
	void Stage_3();
};

extern Stage currentStage;
