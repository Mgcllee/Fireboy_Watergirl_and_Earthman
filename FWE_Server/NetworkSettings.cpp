#pragma once

#include "NetworkSettings.h"

NetworkSettings::NetworkSettings(ULONG open_addr, USHORT open_port) {
  WSADATA WSAData;
  if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
    write_log_message(to_string(WSAGetLastError()));
  }

  SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listenSocket == INVALID_SOCKET) {
    write_log_message(to_string(WSAGetLastError()));
  }

  SOCKADDR_IN server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(open_port);
  server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

  if (bind(listenSocket, reinterpret_cast<sockaddr*>(&server_addr),
             sizeof(server_addr)) == SOCKET_ERROR) {
    write_log_message(to_string(WSAGetLastError()));
  }

  if (listen(listenSocket, 1) == SOCKET_ERROR) {  // 연결할 클라이언트는 1개
    write_log_message(to_string(WSAGetLastError()));
  }
}

NetworkSettings::~NetworkSettings() {}

void NetworkSettings::write_log_message(string message)
{

}

SOCKET* NetworkSettings::get_listen_socket()
{
    // TODO: insert return statement here
    return &listen_socket;
}

void NetworkSettings::close_listen_socket()
{
    closesocket(listen_socket);

}


