#pragma once

#include <array>
#include <vector>
#include <queue>

#include <mutex>

#include <chrono>

#include <utility>

#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")

using namespace std;
using namespace chrono;

#define WINDOW_WID			1200
#define WINDOW_HEI			800

enum STAGE_TYPE {
	STAGE_TITLE = 0,
	STAGE_LOADING,
	STAGE_ROLE,
	STAGE_01,
	STAGE_02,
	STAGE_03,
	RESULT
};