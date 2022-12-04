#pragma once
#include <tchar.h>
#include"Stage.h"
#include"Object.h"
#include"Player.h"
#include<iostream>
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
#define	BTN_STOP			900

#define STAGE_TITLE			0
#define STAGE_LOADING		1
#define STAGE_ROLE			2
#define STAGE_01			3
#define STAGE_02			4
#define STAGE_03			5

// Network Module
#include <wtypes.h>
#pragma comment(lib, "ws2_32")
extern WSADATA	WSAData;
extern SOCKET	c_socket;
//===============

void Move();

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave);
void LoadSound(HWND hWnd);


extern PLAYER players[3];

extern HWND	g_hWnd;
extern MCI_PLAY_PARMS	mciPlayParms;
extern BOOL				keybuffer[256];
extern int currneClientNum;
extern char recvBuf[MAX_BUF_SIZE];
extern int prevSize;
extern int myId;
extern int stageIndex;
extern bool doorVisible;
extern int currentJewelyNum;

extern HANDLE selectMyCharacter;
extern HANDLE changeStageEvent;
extern HANDLE idleStateEvent;
extern HANDLE jumpEvent;
extern Stage currentStage;



bool NetworkInit(HWND& hWnd, std::string input_addr);
void ProcessPacket(char* buf);
void SendPacket(void* buf);
void ConstructPacket(void* buf, int ioSize);
int GetPacketSize(char packetType);
void Display_Err(HWND hWnd, int Errcode);