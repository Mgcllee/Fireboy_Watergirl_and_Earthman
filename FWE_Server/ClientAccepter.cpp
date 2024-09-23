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
		ioctlsocket(client.socket, FIONBIO, &blockingMode);

		int option = TRUE;
		setsockopt(client.socket, IPPROTO_TCP, TCP_NODELAY,
			(const char*)&option, sizeof(option));

		if (client.socket == INVALID_SOCKET) {
			closesocket(client.socket);
			return false;
		} else {
			client.player_state = PLAYER_STATE::PLAYER_ACCEPT;
			ClientAcceptSyncPacket::sync_send_packet(clients, user_ticket);
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
