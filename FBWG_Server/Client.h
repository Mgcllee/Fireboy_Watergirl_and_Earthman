#pragma once

#include "stdafx.h"
#include "PacketSender.h"
#include "StagePosition.h"

class Client : public StagePosition {
public:
  Client();
  ~Client();

  void set_ready_for_play(int user_ticket);
  void run_client_thread(LPVOID arg);


  bool have_role();
  STAGE_TYPE get_curr_stage();

  SOCKET socket;
public:
  bool role;

  char recvBuf[MAX_BUF_SIZE] = {0};
  int currentSize;
  int prevSize = 0;

  char clientId = -1;

  high_resolution_clock::time_point jumpStartTime;
  high_resolution_clock::time_point jumpCurrentTime;

  STAGE_TYPE curr_stage_type;
  int score = 0;

};
