#pragma once
#include<chrono>
#include<wtypes.h>
#include"object.h"
using namespace std;
using namespace chrono;

enum DIRECTION {
	NONE,
	LEFT,
	RIGHT
};

class StagePosition {
public:
	int x, y;
	short ground = 730;
	short hei = 100, wid = 60;

	bool Falling = false;

	DIRECTION direction = NONE;
	float wid_v{};
	float wid_a{};

	float g = 4.5f;
	float v = 0.f;

	HANDLE jumpEventHandle = NULL;
	HANDLE intDoor = NULL;

	OBJECT onBoard;
	bool isJump = false;
	bool isArrive = true;
};
