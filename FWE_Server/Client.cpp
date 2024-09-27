#pragma once

#include "Client.h"
#include "PacketReceiver.h"


Client::Client()
	: role(false)
	, network_socket(INVALID_SOCKET)
{
}

void Client::run_client_thread(array<Client, 3>* member, Stage* stage, int user_ticket) {
	clients = member;
	stage_item = stage;
	
	packet_receiver = new PacketReceiver(clients, stage);

	while ((*clients)[user_ticket].network_socket != INVALID_SOCKET
		&& (*clients)[user_ticket].curr_stage_type != STAGE_TYPE::STAGE_RETRY) {
		int packet_size = recv((*clients)[user_ticket].network_socket, recv_buffer, MAX_BUF_SIZE, 0);
		if (packet_size > 0) {
			packet_receiver->construct_packet(this, packet_size);
		}
	}
	printf("[%d] Client done\n", user_ticket);
}

bool Client::have_role() {
	return role;
}

STAGE_TYPE Client::get_curr_stage() {
	return curr_stage_type;
}
