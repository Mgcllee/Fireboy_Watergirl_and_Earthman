#pragma once
#include<chrono>
#include<wtypes.h>
#include"object.h"
using namespace std;
using namespace chrono;

#define MAX_BUF_SIZE 256

enum DIRECTION {
	NONE,
	LEFT,
	RIGHT
};

class ThreadInfo
{
public:
		HANDLE threadHandle = NULL;
		SOCKET clientSocket;
		char recvBuf[MAX_BUF_SIZE] = { 0 };
		int currentSize;
		int prevSize = 0;
		char clientId = -1;
		short x, y;					// 좌표는 캐릭터 중심
		short ground = 730;
		short hei = 100, wid = 60;	// 캐릭터 크기 (콜라이더 사용)

		bool Falling = false;

		DIRECTION direction = NONE;
		float wid_v{};
		float wid_a{};

		float g = 4.5f;
		float v = 0.f;

		HANDLE jumpEventHandle = NULL;
		HANDLE intDoor = NULL;
		high_resolution_clock::time_point jumpStartTime;
		high_resolution_clock::time_point jumpCurrentTime;

		OBJECT onBoard;
		bool isJump = false;
		bool isArrive = true;
		int score = 0;
};

