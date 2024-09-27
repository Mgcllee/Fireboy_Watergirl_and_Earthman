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
		if (client_pos.isJump || client_pos.Falling) {
			return;
		}

		send_packet.type = static_cast<int>(PACKET_TYPE_S2C::Move_JUMP);
		client_pos.v = 0.f;
		client_pos.isJump = true;
		failling_interpolation(client_pos);
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

		if (false == client_pos.Falling && client_pos.onBoard.FT_Collide_Fall(client_pos)) {
			client_pos.isJump = false;
			client_pos.Falling = true;
			client_pos.jumpStartTime = high_resolution_clock::now();
			client_pos.jumpCurrentTime = high_resolution_clock::now();
			failling_interpolation(client_pos);
		}
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

		if (false == client_pos.Falling && client_pos.onBoard.FT_Collide_Fall(client_pos)) {
			client_pos.isJump = false;
			client_pos.Falling = true;
			client_pos.jumpStartTime = high_resolution_clock::now();
			client_pos.jumpCurrentTime = high_resolution_clock::now();
			failling_interpolation(client_pos);
		}
	}

	send_packet.id = client_pos.user_ticket;
	send_packet.x = client_pos.x;
	send_packet.y = client_pos.y;

	for (Client& client : *clients) {
		send(client.network_socket, reinterpret_cast<char*>(&send_packet), sizeof(send_packet), 0);
	}
}


void C2SMove::failling_interpolation(Client& client) {
	client.jumpStartTime = high_resolution_clock::now();
	client.jumpCurrentTime = high_resolution_clock::now();

	MovePacket mPacket;
	mPacket.type = static_cast<int>(PACKET_TYPE_S2C::Move_JUMP);
	mPacket.size = sizeof(MovePacket);
	mPacket.id = client.user_ticket;

	while (true) {
		if (false == client.Falling && false == client.isJump) {
			client.isJump = false;
			client.Falling = false;
			client.y = client.ground;
			break;
		}

		auto startDuration = high_resolution_clock::now() - client.jumpStartTime;
		auto currentDuration = high_resolution_clock::now() - client.jumpCurrentTime;

		if (client.Falling || duration_cast<milliseconds>(startDuration).count() > 270) {
			if (client.v < FLT_EPSILON)
				client.v = 0.f;

			if (duration_cast<milliseconds>(currentDuration).count() > 30) {
				int prevPosX = client.x;
				if (client.direction == DIRECTION::LEFT) {
					if ((client.x - static_cast<int>(client.wid_v) < WINDOW_WID - client.wid)
						&& (client.x - static_cast<int>(client.wid_v) > client.wid))
						client.x -= static_cast<int>(client.wid_v);
					else
						client.x += static_cast<int>(client.wid_v);
				}
				else if (client.direction == DIRECTION::RIGHT) {
					if ((client.x + static_cast<int>(client.wid_v) < WINDOW_WID - client.wid / 2)
						&& (client.x + static_cast<int>(client.wid_v) > client.wid))
						client.x += static_cast<int>(client.wid_v);
					else
						client.x -= static_cast<int>(client.wid_v);
				}

				if (client.x + 5 >= WINDOW_WID)
					client.x = prevPosX;
				if (client.x - 55 < 0)
					client.x = prevPosX;

				client.v += client.g;
				client.y += static_cast<int>(client.v);

				for (OBJECT& ft : (*stage_item).Ft) {
					if (ft.Ft_Collision(client)) {
						client.direction = DIRECTION::NONE;
						client.v = 0.f;
						client.isJump = false;
						client.Falling = false;
						client.onBoard = ft;
						client.y = client.ground = ft.y - ft.hei;
						mPacket.type = static_cast<int>(PACKET_TYPE_S2C::Move_IDLE);
						break;
					}
				}
				client.jumpCurrentTime = high_resolution_clock::now();

				mPacket.x = client.x;
				mPacket.y = client.y;
				for (Client& send_client : *clients) {
					send(send_client.network_socket, reinterpret_cast<char*>(&mPacket), sizeof(MovePacket), 0);
				}
			}
		}
		else if (duration_cast<milliseconds>(currentDuration).count() > 30 && !client.Falling) {
			int prevPosX = client.x;

			if (client.direction == DIRECTION::LEFT) {
				if ((client.x - static_cast<int>(client.wid_v) < WINDOW_WID - client.wid)
					&& (client.x - static_cast<int>(client.wid_v) > client.wid))
					client.x -= static_cast<int>(client.wid_v);
				else
					client.x += static_cast<int>(client.wid_v);
			}
			else if (client.direction == DIRECTION::RIGHT) {
				if ((client.x + static_cast<int>(client.wid_v) < WINDOW_WID - client.wid / 2)
					&& (client.x + static_cast<int>(client.wid_v) > client.wid))
					client.x += static_cast<int>(client.wid_v);
				else
					client.x -= static_cast<int>(client.wid_v);
			}

			if (client.x + 5 >= WINDOW_WID)
				client.x = prevPosX;
			if (client.x - 55 < 0)
				client.x = prevPosX;

			client.v -= client.g;
			client.y += int(1.3f * client.v);

			for (OBJECT& ft : (*stage_item).Ft) {
				if ((ft.y < client.y) && ft.Collision(client)) {
					client.y -= int(1.3f * client.v);

					client.v = 0.f;
					client.y += ft.hei;
					client.Falling = true;
					break;
				}
			}

			client.jumpCurrentTime = high_resolution_clock::now();

			mPacket.x = client.x;
			mPacket.y = client.y;
			for (Client& send_client : *clients) {
				send(send_client.network_socket, reinterpret_cast<char*>(&mPacket), sizeof(MovePacket), 0);
			}
		}
	}
}