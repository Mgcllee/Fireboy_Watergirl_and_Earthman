#pragma once

#include "stdafx.h"
#include "ServerSettings.h"

class NetworkSettings : public ServerSettings {
public:
	NetworkSettings(ULONG open_addr, USHORT open_port);
	~NetworkSettings();

	void write_log_message(string messgae) override;

	SOCKET* get_listen_socket();

	void close_listen_socket();

private:
	WSADATA WSAData;
	SOCKET listen_socket;
	SOCKADDR_IN server_addr;

};
