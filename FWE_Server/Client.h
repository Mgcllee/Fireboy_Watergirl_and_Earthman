#pragma once

#include "stdafx.h"
#include "PacketSender.h"
#include "PacketReceiver.h"
#include "StagePosition.h"

class Client : public StagePosition {
public:
  Client();

  void set_ready_for_play(int user_ticket);
  void run_client_thread();


  bool have_role();
  STAGE_TYPE get_curr_stage();

  SOCKET socket;
public:
  bool role;

  char recv_buffer[MAX_BUF_SIZE] = {0};
  int currentSize;
  int rest_packet_size = 0;

  char clientId = -1;

  high_resolution_clock::time_point jumpStartTime;
  high_resolution_clock::time_point jumpCurrentTime;

  STAGE_TYPE curr_stage_type;
  int score = 0;

private:
	PacketReceiver packet_receiver;
	PacketSender packet_sender;
};
