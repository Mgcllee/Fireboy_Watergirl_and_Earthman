#pragma once
#include <windows.h>
#include <tchar.h>
#include <atlimage.h>
#include <mmsystem.h>
#include <unordered_map>

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



struct FootHold {
	int x = 0, y = 0;
	int wid = 111, hei = 23;
	bool W_On = FALSE, F_On = FALSE;
};

class PLAYER {
public:
	int x, y;		// 우하단 좌표
	int hei, wid;	// 이미지 크기

	int ground = 730;
	float g = 4;			// 중력 조절로 점프 높이 조정
	float v = 50;
	float wid_a = 0;
	float wid_v = 0;
	int dic = 0;
	short Frame = 0;

	CImage Anim[5]{};
	int C_img_Frame{};
	int C_img_x{}, C_img_y{};
	int C_img_X_Size_01{}, C_img_Y_Size_01{};
	int C_img_X_Size_02{}, C_img_Y_Size_02{};

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
	float v = 0;			// 이동 속도
	bool Down = FALSE;
};

class OBJECT {

public:
	int image_x{}, image_y{};	// 이미지 
	int x{}, y{};				// 윈도우 기준 좌상단 (x, y) 좌표
	int wid{}, hei{};			// 가로길이, 세로길이
	bool On = FALSE;			// 사용 여부

	OBJECT() {};
	OBJECT(int pos_x, int pos_y, int WID, int HEI, bool ON) : x(pos_x), y(pos_y), wid(WID), hei(HEI), On(ON)
	{
		this->image_x = 0;
		this->image_y = 0;
	}

	// 충돌 확인 함수, Player(fire boy or water girl) 입력을 받아 this Object로 충돌 비교
	bool Collision(PLAYER& pl) {
		/*if (	((x - wid <= (pl->x - pl->wid)	&& (pl->x - pl->wid)	<= x + wid) 
			||	(x - wid <=	pl->x				&&	pl->x				<= x + wid))
			&&	((y <= pl->y && pl->y <= y + hei)
			||	(pl->y - pl->hei >= y && pl->y - pl->hei <= y + hei))) {
			On = false;
			return true;
		}*/
	
		if (pl.y <= y - hei) {
			this->On = false;
			// exit(true);
			return true;
		} 

		return false;
	}

	/*
	
	for (int i = 0; i < 20; i++)
				{
					if (i >= 10)
					{  
						if (((	fire.x - 50 <= Jewelry[i].x + 28	&&	 fire.x - 50	>= Jewelry[i].x) 
						|| (	fire.x		<= Jewelry[i].x + 28	&&	 fire.x			>= Jewelry[i].x)) 
						 
						&& ((	fire.y - 80	>= Jewelry[i].y			&& fire.y - 80		<= Jewelry[i].y + 25)
						|| (	fire.y		>= Jewelry[i].y			&& fire.y			<= Jewelry[i].y + 25)))

						{
							Jewelry[i].On = FALSE;
						}
						else if (fire.x - 50 <= Jewelry[i].x && fire.x >= Jewelry[i].x && fire.y - 80 <= Jewelry[i].y && fire.y >= Jewelry[i].y)
						{
							Jewelry[i].On = FALSE;
						}
					}
					if (i < 10)
					{
						if (((water.x - 50 <= Jewelry[i].x + 28 && water.x - 50 >= Jewelry[i].x) || (water.x <= Jewelry[i].x + 28 && water.x >= Jewelry[i].x)) && ((water.y - 80 >= Jewelry[i].y && water.y - 80 <= Jewelry[i].y + 25) || (water.y >= Jewelry[i].y && water.y <= Jewelry[i].y + 25)))
						{
							Jewelry[i].On = FALSE;
						}
						else if (water.x - 50 <= Jewelry[i].x && water.x >= Jewelry[i].x && water.y - 80 <= Jewelry[i].y && water.y >= Jewelry[i].y)
						{
							Jewelry[i].On = FALSE;
						}
					}
				}
				
	
	*/
};

extern PLAYER water;
extern PLAYER fire;

extern MCI_PLAY_PARMS	mciPlayParms;
extern BOOL				keybuffer[256];