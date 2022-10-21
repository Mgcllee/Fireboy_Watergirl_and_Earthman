#include<windows.h>
#include <tchar.h>
#include <atlimage.h>
#include<mmsystem.h>

void Loop();
extern BOOL keybuffer[256];

void Push();
void Jump();
void Wid_Move();
void Foot();
void Climb();

struct FootHold {
	int x = 0, y = 0;
	int wid = 111, hei = 23;
	bool W_On = FALSE, F_On = FALSE;
};

struct PLAYER {
	int x;
	int y;

	int ground = 730;
	float g = 4;			// 중력 조절로 점프 높이 조정
	float v = 50;
	float wid_a = 0;
	float wid_v = 0;
	int dic = 0;

	bool on = TRUE;
	bool is_Move = FALSE;
	bool is_Speed_Down = FALSE;
	bool is_Jumping = FALSE;
	bool is_Push = FALSE;
	bool Down = FALSE;
};

struct REctangle {
	int x;
	int y, dic = 0;

	float g = 4;			// 중력 조절로 점프 높이 조정
	float v = 0;
	bool Down = FALSE;
};

struct Hill {
	int x, y, wid, hei;
	int dic;				// 1 : 우측으로 올라가는 방향, -1 : 좌측으로 올라가는 방향
};

struct Object {
	int image_x, image_y;
	int x, y;
	int wid = 111, hei = 23;
	bool On = FALSE;
};

extern Object Jewelry[20]; // 0~9까지 파랑보석 10~19까지 빨강보석
extern Object Trap[90]; // 0~19까지 파랑 가운데 물 20~29까지 파랑 왼.오 30~49까지 빨강 가운데 물 50~59까지 빨강물 왼.오 60~79 초록 가운데 80~89 초록왼.오
extern Object Die;
extern Object blue_door;
extern Object red_door;
extern Object button[5];
extern Object block[5];

extern FootHold Ft[20];
extern PLAYER water;
extern PLAYER fire;

extern REctangle Rt;
extern Hill hill;



void Stage_1(int );
void Stage_2(int);
void Stage_3(int);
void Stage_4(int);
void Stage_5(int);
void Stage_6(int);