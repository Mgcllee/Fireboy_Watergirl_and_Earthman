#pragma once

#include "Client.h"

Client::Client() 
	: role(false)
{
}

void Client::run_client_thread() {
	while (socket != INVALID_SOCKET) {
		int packet_size = recv(socket, recv_buffer + rest_packet_size, 
			MAX_BUF_SIZE - rest_packet_size, 0);

		if (packet_size > 0) {
			packet_receiver.construct_packet(this, packet_size);
			packet_receiver.process_packet(this, recv_buffer);
		}
	}
}

bool Client::have_role() {
	return role;
}

STAGE_TYPE Client::get_curr_stage() {
	return curr_stage_type;
}

PLAYER_STATE Client::get_player_state()
{
	return player_state;
}
