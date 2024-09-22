#pragma once

#include "stdafx.h"
#include "Client.h"

class ClientAccepter {
 public:
  ClientAccepter(SOCKET& in_listen_socket);
  ~ClientAccepter();

  bool accept_all_client(array<Client, 3>& clients);

private:
  SOCKET listen_socket;
};
