#pragma once

#include "stdafx.h"

class Client {
public:
  Client();
  ~Client();

  void set_ready_for_play(int user_ticket);

public:
  HANDLE threadHandle = NULL;
  SOCKET socket;
  char recvBuf[MAX_BUF_SIZE] = {0};
  int currentSize;
  int prevSize = 0;
  char clientId = -1;
  short x, y;
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
  high_resolution_clock::time_point jumpStartTime;
  high_resolution_clock::time_point jumpCurrentTime;

  OBJECT onBoard;
  bool isJump = false;
  bool isArrive = true;
  int score = 0;
};
