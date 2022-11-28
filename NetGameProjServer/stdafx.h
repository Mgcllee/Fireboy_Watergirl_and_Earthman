#pragma once
#include<iostream>
#include<WS2tcpip.h>
#include<array>
#include<mutex>
#include<chrono>
#include"protocol.h"

#pragma comment(lib, "ws2_32")

using namespace std;
using namespace chrono;
#define MAX_BUF_SIZE 256

#define STAGE_TITLE			0
#define STAGE_LOADING		1
#define STAGE_ROLE			2
#define STAGE_01			3
#define STAGE_02			4
#define STAGE_03			5

struct threadInfo {
	HANDLE threadHandle = NULL;
	SOCKET clientSocket;
	char recvBuf[MAX_BUF_SIZE] = { 0 };
	int currentSize;
	int prevSize = 0;
	char clientId = -1;
	short x, y, ground = 730;	// 좌표는 캐릭터 중심
	short hei = 100, wid = 60;	// 캐릭터 크기

	int direction;
	float wid_v{};
	float wid_a{};

	float g = 4.f;
	float v = 0.f;

	HANDLE jumpEventHandle = NULL;
	high_resolution_clock::time_point jumpStartTime;
	high_resolution_clock::time_point jumpCurrentTime;
	bool isJump = false;
};

void Display_Err(int Errcode);
void ConstructPacket(threadInfo& clientInfo, int ioSize); // 패킷 재조립
void ProcessPacket(threadInfo& clientInfo, char* packetStart); // 패킷 재조립 후, 명령 해석 후 행동
int GetPacketSize(char packetType);

void ChangeRole(); // mutex 필요 없을듯? => change는 딱히 문제 없다고 생각함
void SelectRole(); // mutex 필요 => 두 클라이언트가 동시에 같은 케릭터 선택을 해버리면 안됨
//void MovePacket(); // 움직일 때 마다, 전송
void CheckJewelryEat();// 쥬얼리 습득 확인
void CheckOpenDoor(); // 문 열리는 조건 확인


extern array<threadInfo, 3> threadHandles;
extern array<char, 3> playerRole;
extern mutex selectMutex;
extern array<char, 3> selectPlayerRole;
extern HANDLE multiEvenTthreadHadle[3];

extern int stageIndex;

//HANDLE loadFlag; =>웨이트포실긍