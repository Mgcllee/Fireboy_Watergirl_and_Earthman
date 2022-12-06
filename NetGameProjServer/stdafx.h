#pragma once
#include<iostream>
#include<WS2tcpip.h>
#include<array>
#include<mutex>
#include<chrono>
#include"ThreadInfo.h"
#include"object.h"
#include"protocol.h"

#pragma comment(lib, "ws2_32")

using namespace std;
using namespace chrono;

#define STAGE_TITLE			0
#define STAGE_LOADING		1
#define STAGE_ROLE			2
#define STAGE_01			3
#define STAGE_02			4
#define STAGE_03			5
#define RESULT				6


void Display_Err(int Errcode);
void ConstructPacket(ThreadInfo& clientInfo, int ioSize); // 패킷 재조립
void ProcessPacket(ThreadInfo& clientInfo, char* packetStart); // 패킷 재조립 후, 명령 해석 후 행동
int GetPacketSize(char packetType);

void ChangeRole(); // mutex 필요 없을듯? => change는 딱히 문제 없다고 생각함
void SelectRole(); // mutex 필요 => 두 클라이언트가 동시에 같은 케릭터 선택을 해버리면 안됨
//void MovePacket(); // 움직일 때 마다, 전송
void CheckJewelryEat();// 쥬얼리 습득 확인
void CheckOpenDoor(); // 문 열리는 조건 확인


extern array<ThreadInfo, 3> threadHandles;
extern array<char, 3> playerRole;
extern mutex selectMutex;
extern array<char, 3> selectPlayerRole;
extern HANDLE multiEvenTthreadHadle[3];

extern int stageIndex;

//HANDLE loadFlag; =>웨이트포실긍