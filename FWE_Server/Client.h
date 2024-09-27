#pragma once

#include "stdafx.h"
#include "Stage.h"
// #include "PacketReceiver.h"
#include "StagePosition.h"

#ifndef CLIENT_H
#define CLIENT_H

class PacketReceiver;

class Client : public StagePosition {
public:
	Client();

	void run_client_thread(array<Client, 3>* member, Stage* stage, int user_ticket);


	bool have_role();
	STAGE_TYPE get_curr_stage();

	SOCKET network_socket;

	PLAYER_STATE player_state;

public:
	bool role;

	char recv_buffer[MAX_BUF_SIZE] = { 0 };
	int currentSize;
	int rest_packet_size = 0;

	char user_ticket = -1;

	high_resolution_clock::time_point jumpStartTime;
	high_resolution_clock::time_point jumpCurrentTime;

	STAGE_TYPE curr_stage_type;
	int score = 0;

private:
	array<Client, 3>* clients;
	Stage* stage_item;
	PacketReceiver* packet_receiver;
};

#endif 
