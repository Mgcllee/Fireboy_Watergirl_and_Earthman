#pragma once

#include "stdafx.h"
#include "protocol.h"
#include "StagePosition.h"
#include "Client.h"

class PacketReceiver {
public:
	virtual void recv_packet(Client& client, void* packet);
};

class ClientSelectRole : public PacketReceiver {
public:
	void recv_packet(Client& client, void* packet) override;
};

class C2SChangRole : public PacketReceiver {
public:
	void recv_packet(Client& client, void* packet) override;
};

class C2SMove : public PacketReceiver {
public:
	void recv_packet(Client& client, void* packet) override;
};