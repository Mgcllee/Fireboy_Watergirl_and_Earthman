#include "ClientAccepter.h"

ClientAccepter::ClientAccepter(SOCKET* in_listen_socket)
	: listen_socket(in_listen_socket) {

}

ClientAccepter::~ClientAccepter() {}

bool ClientAccepter::accept_all_client(array<Client, 3>* clients) {	
	int user_ticket = 0;
	for (Client& client : *clients) {
		if (client.network_socket != INVALID_SOCKET) {
			user_ticket += 1;
			continue;
		}
		
		SOCKADDR_IN client_addr;
		int addr_size = sizeof(client_addr);

		client.network_socket = accept(
			*listen_socket, reinterpret_cast<sockaddr*>(&client_addr), &addr_size);

		if (client.network_socket == INVALID_SOCKET) {
			closesocket(client.network_socket);
			return false;
		} else {
			u_long blockingMode = 1;
			ioctlsocket(client.network_socket, FIONBIO, &blockingMode);

			int option = TRUE;
			setsockopt(client.network_socket, IPPROTO_TCP, TCP_NODELAY,
				(const char*)&option, sizeof(option));

			client.user_ticket = user_ticket;
			client.player_state = PLAYER_STATE::PLAYER_ACCEPT;
			ClientAcceptSyncPacket client_accepter(clients);
			client_accepter.sync_send_packet(&user_ticket);
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
