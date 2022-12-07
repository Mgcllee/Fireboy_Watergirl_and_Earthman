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

#define WINDOW_WID			1200
#define WINDOW_HEI			800

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

extern array<ThreadInfo, 3> threadHandles;
extern array<char, 3> playerRole;
extern mutex selectMutex;
extern array<char, 3> selectPlayerRole;
extern HANDLE multiEvenTthreadHadle[3];

//HANDLE loadFlag; =>웨이트포실긍