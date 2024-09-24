#pragma once

#include "stdafx.h"
#include "Client.h"
#include "StagePosition.h"



class PacketSender {
public:
	PacketSender(array<Client, 3>* member);

	virtual void send_packet(Client& client, void* packet);
	virtual void sync_send_packet(void* packet);

	array<Client, 3>* clients;
};

class StageUpdatePacket : public PacketSender {
public:
	StageUpdatePacket(array<Client, 3>* member);

	void sync_send_packet(void* packet) override;
};

class ClientMovePacket : public PacketSender {
public:
	ClientMovePacket(array<Client, 3>* member);

	void sync_send_packet(void* next_position) override;
};

class ClientAcceptSyncPacket : public PacketSender {
public:
	ClientAcceptSyncPacket(array<Client, 3>* member);

	void sync_send_packet(void* packet) override;
};

class StageDoorOpenSyncPacket : public PacketSender {
public:
	StageDoorOpenSyncPacket(array<Client, 3>* member);

	void sync_send_packet(void* packet) override;
};