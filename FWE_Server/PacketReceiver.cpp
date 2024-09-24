#pragma once

#include "PacketReceiver.h"
#include "Client.h"


PacketReceiver::PacketReceiver(array<Client, 3>* member, Stage* game_stage)
	: clients(member)
	, stage_item(game_stage)
{
}

void PacketReceiver::construct_packet(Client* client, int recv_packet_size) {
	int rest_size = recv_packet_size + client->rest_packet_size;
	int curr_packet_size = 0;

	char* recv_buufer = reinterpret_cast<char*>(client->recv_buffer);
	while (rest_size != 0) {
		curr_packet_size = (recv_buufer[1]);
		if (rest_size < curr_packet_size) {
			client->rest_packet_size = rest_size;
			return;
		} else {
			process_packet(recv_buufer);
			memcpy(recv_buufer, recv_buufer + curr_packet_size, rest_size - curr_packet_size);
			rest_size -= curr_packet_size;
		}
	}
}

void PacketReceiver::process_packet(char* packet) {
	switch (static_cast<PACKET_TYPE_C2S>(packet[0])) {
	case PACKET_TYPE_C2S::SelectRole: {
		ClientSelectRole select_role(clients, stage_item);
		select_role.recv_sync_packet(packet);
		break;
		}
	case PACKET_TYPE_C2S::ChangRole: {
		C2SChangRole change_role(clients, stage_item);
		change_role.recv_sync_packet(packet);
		break;
		}
	case PACKET_TYPE_C2S::Move: {
		C2SMove move(clients, stage_item);
		move.recv_sync_packet(packet);
		break;
		}
	case PACKET_TYPE_C2S::Endout: {
		for (Client& client : *clients) {
			closesocket(client.network_socket);
		}
		break;
		}
	}
}

void PacketReceiver::recv_packet(void* packet)
{
}

void PacketReceiver::recv_sync_packet(void* packet)
{
}

ClientSelectRole::ClientSelectRole(array<Client, 3>* member, Stage* game_stage)
	: PacketReceiver(member, game_stage)
{
}

void ClientSelectRole::recv_sync_packet(void* recv_packet)
{
	C2SRolePacket* packet = reinterpret_cast<C2SRolePacket*>(recv_packet);
	int user_ticket = static_cast<int>(packet->id);
	Client select_client = (*clients)[user_ticket];

	// TODO: update select palyer role varialbe logic
	bool change = true;
	/*for (int i = 0; i < 3; i++) {
		if (selectPlayerRole[i].load() == packet->role) {
			change = false;
			selectPlayerRole[select_client.user_ticket].store(packet->role);
			break;
		}
	}*/
	
	if (change) {
		S2CRolePacket sendPacket;
		sendPacket.id = select_client.user_ticket;
		sendPacket.role = packet->role;
		sendPacket.type = static_cast<int>(PACKET_TYPE_S2C::SelectRole);

		for (Client& client : *clients) {
			send(client.network_socket, reinterpret_cast<char*>(&sendPacket), sizeof(S2CRolePacket), 0);
		}
	}
}

C2SChangRole::C2SChangRole(array<Client, 3>* member, Stage* game_stage)
	: PacketReceiver(member, game_stage)
{
}

void C2SChangRole::recv_sync_packet(void* packetStart)
{
	C2SRolePacket* packet = reinterpret_cast<C2SRolePacket*>(packetStart);
	int user_ticket = static_cast<int>(((*clients)[packet->id]).user_ticket);

	// TODO: playerRole update
	// playerRole[user_ticket].store(packet->role);

	S2CRolePacket sendPacket;
	sendPacket.id = user_ticket;
	sendPacket.role = packet->role;
	sendPacket.type = static_cast<int>(PACKET_TYPE_S2C::ChangeRole);

	for (Client& client : *clients) {
		send(client.network_socket, reinterpret_cast<char*>(&sendPacket), sizeof(sendPacket), 0);
	}
}

C2SMove::C2SMove(array<Client, 3>* member, Stage* game_stage)
	: PacketReceiver(member, game_stage)
{
}

void C2SMove::recv_sync_packet(void* position_packet)
{
	MovePacket* packet = reinterpret_cast<MovePacket*>(position_packet);
	Client& client_pos = (*clients)[packet->id];
	short prevPosX = client_pos.x;
	
	if (packet->y == SHRT_MAX) {
		packet->type = static_cast<int>(PACKET_TYPE_S2C::Move_JUMP);
		client_pos.v = 0.f;
	}
	else if (packet->y == SHRT_MIN) {
		client_pos.direction = DIRECTION::NONE;
		packet->type = static_cast<int>(PACKET_TYPE_S2C::Move_IDLE);
		client_pos.wid_v = 0;
	}
	else if (packet->x == 1) {
		if (client_pos.wid_a <= 10.f)
			client_pos.wid_a += 0.1f;
		if (client_pos.wid_v <= 10.f)
			client_pos.wid_v += client_pos.wid_a;
		Client temp = client_pos;
		temp.x += static_cast<int>(temp.wid_v);
		
		for (OBJECT& ft : (*stage_item).Ft) {
			if (ft.Ft_Collision(temp) && ft.Ft_Collision(client_pos)) {
				client_pos.x -= static_cast<int>(client_pos.wid_v);
				client_pos.wid_v = 0;
			}
		}

		if (client_pos.wid_v != 0) {
			client_pos.x += static_cast<int>(client_pos.wid_v);
		}

		if (client_pos.x + 5 >= WINDOW_WID) {
			client_pos.x = prevPosX;
		}

		client_pos.direction = DIRECTION::RIGHT;
		packet->type = static_cast<int>(PACKET_TYPE_S2C::Move_RIGHT);
	}
	else if (packet->x == -1) {
		if (client_pos.wid_a <= 10.f) {
			client_pos.wid_a += 0.1f;
		}
		if (client_pos.wid_v <= 10.f) {
			client_pos.wid_v += client_pos.wid_a;
		}

		Client temp = client_pos;
		temp.x -= static_cast<int>(temp.wid_v);

		for (OBJECT ft : (*stage_item).Ft) {
			if (ft.Ft_Collision(temp) && ft.Ft_Collision(client_pos)) {
				client_pos.x += static_cast<int>(client_pos.wid_v);
				client_pos.wid_v = 0;
			}
		}

		if (client_pos.wid_v != 0) {
			client_pos.x -= static_cast<int>(client_pos.wid_v);
		}

		if (client_pos.x - 55 < 0) {
			client_pos.x = prevPosX;
		}
		client_pos.direction = DIRECTION::LEFT;
		packet->type = static_cast<int>(PACKET_TYPE_S2C::Move_LEFT);
	}

	packet->x = client_pos.x;
	packet->y = client_pos.y;

	for (Client& client : *clients) {
		send(client.network_socket, reinterpret_cast<char*>(packet), sizeof(packet), 0);
	}
}
