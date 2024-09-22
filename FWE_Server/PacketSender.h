#pragma once

#include "stdafx.h"
#include "protocol.h"
#include "StagePosition.h"

class PacketSender {
public:
	virtual void send_packet(void* packet);
};

class StageUpdatePacket : public PacketSender {
public:
	void send_packet(SOCKET socket, void* stage_index) override;
};

class ClientMovePacket : public PacketSender {
public:
	void send_packet(SOCKET socket, void* new_position) override;
};
