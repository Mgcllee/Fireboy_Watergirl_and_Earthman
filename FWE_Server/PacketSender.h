#pragma once

#include "stdafx.h"
#include "protocol.h"
#include "Client.h"
#include "StagePosition.h"

class PacketSender {
public:
	virtual void send_packet(Clinet& client, void* packet);
	virtual void sync_send_packet(void* packet);
};

class StageUpdatePacket : public PacketSender {
public:
	void send_packet(Clinet& socket, void* stage_index) override;
};

class ClientMovePacket : public PacketSender {
public:
	void send_packet(Clinet& socket, void* new_position) override;
};

class ClientServerIDPacket : public PacketSender {
public:
	void send_packet(Clinet& socket, void* user_ticket) override;
}; 