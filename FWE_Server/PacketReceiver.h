#pragma once

#include "stdafx.h"
#include "protocol.h"
#include "StagePosition.h"
#include "Client.h"

class PacketReceiver {
public:
	void construct_packet(Client& client, int recv_packet_size);
	void process_packet(Client& clinet, char* packet);
	
protected:
	virtual void recv_packet(Client& client, void* packet);
};

class ClientSelectRole : protected PacketReceiver {
protected:
	void recv_packet(Client& client, void* packet) override;
};

class C2SChangRole : protected PacketReceiver {
protected:
	void recv_packet(Client& client, void* packet) override;
};

class C2SMove : protected PacketReceiver {
protected:
	void recv_packet(Client& client, void* packet) override;
};