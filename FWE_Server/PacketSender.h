#pragma once

#include "stdafx.h"
#include "protocol.h"
#include "Client.h"
#include "StagePosition.h"

class PacketSender {
public:
	virtual void send_packet(Clinet& client, void* packet);
	virtual void sync_send_packet(array<Client, 3>& clients, void* packet);
};

class StageUpdatePacket : public PacketSender {
public:
	void sync_send_packet(array<Client, 3>& clients, void* packet) override;
};

class ClientMovePacket : public PacketSender {
public:
	void sync_send_packet(array<Client, 3>& clients, void* new_positions) override;
};

class ClientAcceptSyncPacket : public PacketSender {
public:
	void sync_send_packet(array<Client, 3>& clients, void* packet) override;
}; 