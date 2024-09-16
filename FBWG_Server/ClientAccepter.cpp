#include "ClientAccepter.h"

ClientAccepter::ClientAccepter(SOCKET& in_listen_socket)
	: listen_socket(in_listen_socket) {

}

ClientAccepter::~ClientAccepter() {}

bool ClientAccepter::accept_all_client(array<Client, 3>& clients) {
	
	int user_ticket = 0;
	for (Client& client : clients) {
		SOCKADDR_IN client_addr;
		int addr_size = sizeof(client_addr);
		client.socket = accept(
			listen_socket, reinterpret_cast<sockaddr*>(&client_addr), &addr_size);

		u_long blockingMode = 1;
		ioctlsocket(client.clientSocket, FIONBIO, &blockingMode);

		int option = TRUE;
		setsockopt(client.clientSocket, IPPROTO_TCP, TCP_NODELAY,
			(const char*)&option, sizeof(option));

		if (client.clientSocket == INVALID_SOCKET) {
			Display_Err(WSAGetLastError());
			closesocket(listen_socket);
			WSACleanup();
			return false;
		}
		else {
			client.set_ready_for_play(user_ticket);
			user_ticket += 1;
		}
	}
	if (user_ticket == 3) {
		return true;
	}
	else {
		return false;
	}
}
