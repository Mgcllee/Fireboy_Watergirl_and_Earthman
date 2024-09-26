#pragma once

#include "Stage.h"
#include "StagePosition.h"

#ifndef PACKET_RECEIVER_H
#define PACKET_RECEIVER_H

class Client;

class PacketReceiver {
public:
	PacketReceiver(array<Client, 3>* member, Stage* game_stage);

	void construct_packet(Client* client, int recv_packet_size);
	void process_packet(char* packet);
	
	virtual void recv_packet(void* packet);
	virtual void recv_sync_packet(void* packet);

	array<Client, 3>* clients;
	Stage* stage_item;
};

class ClientSelectRole : public PacketReceiver {
public:
	ClientSelectRole(array<Client, 3>* member, Stage* game_stage);

	void recv_sync_packet(void* packet) override;
};

class C2SChangRole : public PacketReceiver {
public:
	C2SChangRole(array<Client, 3>* member, Stage* game_stage);

	void recv_sync_packet(void* packet) override;
};

class C2SMove : public PacketReceiver {
public:
	C2SMove(array<Client, 3>* member, Stage* game_stage);

	void recv_sync_packet(void* packet) override;

	void failling_interpolation(Client& client_pos);
};

#endif