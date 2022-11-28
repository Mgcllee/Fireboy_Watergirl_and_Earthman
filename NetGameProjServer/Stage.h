#pragma once
#include <iostream>
#include <vector>

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

class OBJECT {
	bool On = false;			// 사용 여부

public:
	int x{}, y{};				// 윈도우 기준 좌상단 (x, y) 위치좌표
	int image_x{}, image_y{};	// 출력할 이미지 프레임 기준점 (스프라이트 이미지)
	int wid{}, hei{};			// 가로길이, 세로길이
	int MaxWid{};				// 전체 스프라이트 가로길이
	int imageMoveWid{};			// 스프라이트 이미지 이동

	OBJECT() {};
	OBJECT(int pos_x, int pos_y, int WID, int HEI, int MAXWID, int IMAGEMOVEWID, bool ON)
		: x(pos_x), y(pos_y), wid(WID), hei(HEI), MaxWid(MAXWID), imageMoveWid(IMAGEMOVEWID), On(ON) {}

	// 충돌 확인 함수, Player(fire boy or water girl) 입력을 받아 this Object로 충돌 비교
	bool Collision(PLAYER& pl) {
		if (
			(abs((x - wid / 2) - (pl.x - pl.wid / 2)) <= (wid + pl.wid) / 2)
			&&
			(abs((y - hei / 2) - (pl.y - pl.hei / 2)) <= (hei + pl.hei) / 2)
			) {
			return true;
		}
		else return false;
	}

	bool Ft_Collision(PLAYER& pl) {
		if (
			(abs((x - wid / 2) - (pl.x - pl.wid / 2)) < (wid + pl.wid) / 2)
			&&
			(abs((y - hei / 2) - (pl.y - pl.hei / 2)) < (hei + pl.hei) / 2)
			) {
			return true;
		}
		else return false;
	}

	void SetVisible(bool in) {
		On = in;
	}

	bool GetVisible() {
		return On;
	}

	bool ChangeFrame(int direction, bool replay) {
		if (MaxWid == image_x + imageMoveWid) {
			if (replay)
				image_x = 0;
			return true;
		}
		else {
			image_x += (imageMoveWid * direction);
			return false;
		}
	}
};

class Stage
{
public:
	// RECTANGLE Rt;

	short stage;						// 현재 스테이지
	bool clear;							// Stage 클리어 여부
	int average;
	bool time_over;						// 현재 Stage에서 Timer Over 여부
	int count;							// 애니메이션 프레임 번호
	int red_total;						// 빨강 보석 총 개수
	int blue_total;						// 파랑 보석 총 개수
	bool stair;							// Fire boy와 Water girl 이 문 안 계단을 올라감

	OBJECT Ground{ 0, GROUND_POS_Y, WINDOW_WID, WINDOW_HEI - GROUND_POS_Y, 0, 0, true };
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
	void Stage_1();
	void Stage_2();
	void Stage_3();
};

extern Stage currentStage;
