#pragma once

#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>
#include <atlimage.h>
#include <mmsystem.h>
#include <unordered_map>
#include <array>

#pragma comment(lib,"winmm.lib")
#pragma comment (lib, "msimg32.lib")

#define BTN_START 100
#define BTN_RESTART 200
#define BTN_QUIT 300
#define BTN_NEXT_STAGE 350
#define EDIT_SERVER_ADDR 400
#define BTN_LEFT_ARROW 500
#define BTN_RIGHT_ARROW 550
#define BTN_SELECT 600



// Network Module
#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")
extern WSADATA WSAData;
extern SOCKET c_socket;
//===============


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
	int x, y;					// 우하단 좌표
	int hei = 100, wid = 60;	// 캐릭터 크기

	int ground = 730;		// Stage의 바닥 높이 (상단이 y = 0, 내려갈수록 +)
	float g = 4;			// 중력 조절로 점프 높이 조정
	float v = 50;			// 속도
	float wid_a = 0;		// 좌우 가속도
	float wid_v = 0;		// 좌우 속도
	int dic = 0;			// 이동 방향
	short Frame = 0;		// 애니메이션 프레임

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
	bool On = FALSE;			// 사용 여부

public:
	int image_x{}, image_y{};	// 이미지 프레임 기준점 (스프라이트 이미지)
	int x{}, y{};				// 윈도우 기준 좌상단 (x, y) 좌표
	int wid{}, hei{};			// 가로길이, 세로길이

	OBJECT() {};
	OBJECT(int pos_x, int pos_y, int WID, int HEI, bool ON) : x(pos_x), y(pos_y), wid(WID), hei(HEI), On(ON)
	{
		this->image_x = 0;
		this->image_y = 0;
	}

	// 충돌 확인 함수, Player(fire boy or water girl) 입력을 받아 this Object로 충돌 비교
	bool Collision(PLAYER& pl) {
		if  (
			(abs((x - wid / 2) - (pl.x - pl.wid / 2)) <= (wid + pl.wid) / 2)
			&& 
			(abs((y - hei / 2) - (pl.y - pl.hei / 2)) <= (hei + pl.hei) / 2)
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
};

//client Info
struct clientInfo {
	int id;// 다른 클라이언트 식별 정보
	short role; // 어떤 캐릭터 인지

	//좌표가 필요 하겠지?
};

extern PLAYER water;
extern PLAYER fire;

extern MCI_PLAY_PARMS	mciPlayParms;
extern BOOL				keybuffer[256];
extern int currneClientNum;

bool NetworkInit(HWND& hWnd, std::string input_addr);
void send_packet(char* buf);
void ProcessPacket();
