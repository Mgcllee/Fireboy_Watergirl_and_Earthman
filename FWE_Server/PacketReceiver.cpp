#pragma once

#include "PacketReceiver.h"
#include "Client.h"


PacketReceiver::PacketReceiver(array<Client, 3>* member, Stage* game_stage)
	: clients(member)
	, stage_item(game_stage)
{
}

void PacketReceiver::construct_packet(Client* client, int recv_packet_size) {
	int curr_packet_size = 0;

	char* recv_buufer = reinterpret_cast<char*>(client->recv_buffer);
	while (recv_packet_size != 0) {
		curr_packet_size = (recv_buufer[1]);
		if (recv_packet_size < curr_packet_size) {
			return;
		} else {
			process_packet(recv_buufer);
			memcpy(recv_buufer, recv_buufer + curr_packet_size, recv_packet_size - curr_packet_size);
			recv_packet_size -= curr_packet_size;
		}
	}
}

void PacketReceiver::process_packet(char* packet) {
	PACKET_TYPE_C2S packet_type = static_cast<PACKET_TYPE_C2S>(packet[0]);
	switch (packet_type) {
	case PACKET_TYPE_C2S::SelectRole: {
		ClientSelectRole select_role(clients, stage_item);
		select_role.stage_item = stage_item;
		select_role.recv_sync_packet(packet);
		break;
		}
	case PACKET_TYPE_C2S::ChangRole: {
		C2SChangRole change_role(clients, stage_item);
		change_role.stage_item = stage_item;
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

	if ((*stage_item->playerRole)[user_ticket].load() == packet->role) {
		S2CRolePacket sendPacket;
		sendPacket.id = user_ticket;
		sendPacket.role = packet->role;
		sendPacket.type = static_cast<int>(PACKET_TYPE_S2C::SelectRole);

		(*clients)[user_ticket].role = true;

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
	int user_ticket = static_cast<int>(packet->id);

	char select_role = packet->role;

	// TODO: playerRole update
	(*stage_item->playerRole)[user_ticket].store(select_role);

	S2CRolePacket sendPacket;
	sendPacket.id = user_ticket;
	sendPacket.role = packet->role;
	sendPacket.type = static_cast<int>(PACKET_TYPE_S2C::ChangeRole);

	for (Client& client : *clients) {
		if (user_ticket != client.user_ticket) {
			send(client.network_socket, reinterpret_cast<char*>(&sendPacket), sizeof(sendPacket), 0);
		}
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
	
	MovePacket send_packet;
	send_packet.size = sizeof(MovePacket);

	if (packet->y == SHRT_MAX) {
		send_packet.type = static_cast<int>(PACKET_TYPE_S2C::Move_JUMP);
		client_pos.v = 0.f;
		client_pos.isJump = true;
	}
	else if (packet->y == SHRT_MIN) {
		client_pos.direction = DIRECTION::NONE;
		send_packet.type = static_cast<int>(PACKET_TYPE_S2C::Move_IDLE);
		client_pos.wid_a = 0.f;
		client_pos.wid_v = 0.f;
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
		send_packet.type = static_cast<int>(PACKET_TYPE_S2C::Move_RIGHT);
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
		send_packet.type = static_cast<int>(PACKET_TYPE_S2C::Move_LEFT);
	}

	send_packet.id = client_pos.user_ticket;
	send_packet.x = client_pos.x;
	send_packet.y = client_pos.y;

	for (Client& client : *clients) {
		send(client.network_socket, reinterpret_cast<char*>(&send_packet), sizeof(send_packet), 0);
	}
}
