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

#define MAX_BUF_SIZE		256

#define BTN_START			100
#define BTN_RESTART			200
#define BTN_QUIT			300
#define BTN_NEXT_STAGE		400
#define EDIT_SERVER_ADDR	500
#define BTN_LEFT_ARROW		600
#define BTN_RIGHT_ARROW		701
#define BTN_SELECT			800

#define STAGE_TITLE			0
#define STAGE_LOADING		1
#define STAGE_ROLE			2
#define STAGE_01			3
#define STAGE_02			4
#define STAGE_03			5

#define WINDOW_WID			1200
#define WINDOW_HEI			800
#define GROUND_POS_Y		730

// Network Module
#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")
extern WSADATA	WSAData;
extern SOCKET	c_socket;
//===============

void Move();

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave);
void LoadSound(HWND hWnd);

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

	CImage Anim[5]{};
	int C_img_Frame{};
	int C_img_x{}, C_img_y{};
	int C_img_X_Size_01{}, C_img_Y_Size_01{};
	int C_img_X_Size_02{}, C_img_Y_Size_02{};

	bool on = TRUE;
	bool is_Jumping = FALSE;
	bool is_Push = FALSE;
	bool Down = FALSE;

	PLAYER() : id(-1), role('f') {}
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
			if(replay)
				image_x = 0;
			return true;
		}
		else {
			image_x += (imageMoveWid * direction);
			return false;
		}
	}
};

extern PLAYER players[3];

extern HWND	g_hWnd;
extern MCI_PLAY_PARMS	mciPlayParms;
extern BOOL				keybuffer[256];
extern int currneClientNum;
extern char recvBuf[MAX_BUF_SIZE];
extern int prevSize;
extern int myId;
extern int stageIndex;

extern HANDLE selectMyCharacter;
extern HANDLE changeStageEvent;


bool NetworkInit(HWND& hWnd, std::string input_addr);
void ProcessPacket(char* buf);
void SendPacket(void* buf);
void ConstructPacket(void* buf, int ioSize);
int GetPacketSize(char packetType);
void Display_Err(HWND hWnd, int Errcode);