#pragma once
#include <atlimage.h>
#include<wtypes.h>
enum DIRECTION {
	IDLE,
	JUMP,
	LEFT,
	RIGHT
};

class PLAYER {
public:
	int id;						// 다른 클라이언트 식별 정보 - -1 // 아직 안쓰는 상황에 -1
	short role;					// 어떤 캐릭터 인지 - f w e => fire water earth
	int x, y;					// 우하단 좌표
	int hei = 100, wid = 60;	// 캐릭터 크기

	int direction = DIRECTION::IDLE;				// 이동방향 (애니메이션에서 사용)
	float wid_v{};
	float wid_a{};

	int ground = 730;		// Stage의 바닥 높이 (상단이 y = 0, 내려갈수록 +)
	float g = 3.f;			// 중력 조절로 점프 높이 조정
	float v = 0;			// 속도
	short Frame = 0;		// 애니메이션 프레임

	CImage Anim[5];
	int C_img_Frame{};
	int C_img_x{}, C_img_y{};
	int C_img_X_Size_01{}, C_img_Y_Size_01{};
	int C_img_X_Size_02{}, C_img_Y_Size_02{};

	bool on = TRUE;
	bool is_Jumping = FALSE;
	bool is_Push = FALSE;
	bool Down = FALSE;
	bool isIntoDoor = FALSE;
	int score = 0;

	PLAYER() : id(-1), role('f') {}
	~PLAYER() {

	}
};
