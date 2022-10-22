#pragma once
#include<windows.h>
#include <tchar.h>
#include <atlimage.h>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")
#pragma comment (lib, "msimg32.lib")

#define IDC_BUTTON1 100
#define IDC_BUTTON2 200
#define IDC_BUTTON3 300
#define IDC_BUTTON4 350

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave);
void LoadSound(HWND hWnd);

void Loop(bool keyDown);

void Push();
void Jump();
void Wid_Move();
void Foot();

struct FootHold {
	int x = 0, y = 0;
	int wid = 111, hei = 23;
	bool W_On = FALSE, F_On = FALSE;
};

class PLAYER {
public:
	int x;
	int y;

	int ground = 730;
	float g = 4;			// 중력 조절로 점프 높이 조정
	float v = 50;
	float wid_a = 0;
	float wid_v = 0;
	int dic = 0;
	short Frame = 0;

	CImage Anim[5];
	int C_img_Frame;
	int C_img_x, C_img_y;
	int C_img_X_Size_01, C_img_Y_Size_01;
	int C_img_X_Size_02, C_img_Y_Size_02;

	bool on = TRUE;
	bool is_Move = FALSE;
	bool is_Speed_Down = FALSE;
	bool is_Jumping = FALSE;
	bool is_Push = FALSE;
	bool Down = FALSE;

	PLAYER() {

	}
	~PLAYER() {

	}
};

struct RECTANGLE {
	int x;
	int y, dic = 0;

	float g = 4;			// 중력 조절로 점프 높이 조정
	float v = 0;
	bool Down = FALSE;
};

struct OBJECT {
	int image_x, image_y;
	int x, y;
	int wid = 111, hei = 23;
	bool On = FALSE;
};

extern PLAYER water;
extern PLAYER fire;

extern MCI_PLAY_PARMS	mciPlayParms;
extern BOOL				keybuffer[256];