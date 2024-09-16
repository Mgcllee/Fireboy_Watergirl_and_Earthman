#pragma once

#include "stdafx.h"
#include "ServerSettings.h"

class NetworkSettings : public ServerSettings {
 public:
  NetworkSettings(ULONG open_addr, USHORT open_port);
  ~NetworkSettings();

  override void write_log_message(string message);

  SOCKET& get_listen_socket();

 private:
  WSADATA WSAData;
  SOCKET listen_socket;
  SOCKADDR_IN server_addr;

};
