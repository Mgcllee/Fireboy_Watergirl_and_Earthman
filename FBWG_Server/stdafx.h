#pragma once
#include<iostream>
#include<WS2tcpip.h>
#include<array>
#include<mutex>
#include<chrono>

#pragma comment(lib, "ws2_32")

using namespace std;
using namespace chrono;

#define WINDOW_WID			1200
#define WINDOW_HEI			800

enum STAGE_NAME {
	STAGE_TITLE = 0,
	STAGE_LOADING,
	STAGE_ROLE,
	STAGE_01,
	STAGE_02,
	STAGE_03,
	RESULT
};