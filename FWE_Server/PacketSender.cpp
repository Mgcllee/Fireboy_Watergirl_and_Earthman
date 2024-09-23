#pragma once

#include "PacketSender.h"


void PacketSender::send_packet(Client& client, void* packet)
{
}

void PacketSender::sync_send_packet(array<Client, 3>& clients, void* packet)
{
}

void StageUpdatePacket::sync_send_packet(array<Client, 3>& clients, void* stage_index) {
	S2CChangeStagePacket changePacket;
	changePacket.stageNum = static_cast<int>(stage_index);
	changePacket.type = PACKET_TYPE_S2C::ChangeStage;

	for (Client& client : clients) {
		send(client->network_socket, reinterpret_cast<const char*>(&changePacket), sizeof(S2CChangeStagePacket), 0);
	}
}

void ClientMovePacket::sync_send_packet(array<Client, 3>& clients, void* next_position)
{
	StagePosition position = static_cast<StagePosition>(next_position);
	
	MovePacket setPosition;
	setPosition.type = PACKET_TYPE_S2C::Move_IDLE;
	setPosition.x = position.x;
	setPosition.y = position.y;

	for (Client& client : clients) {
		send(client.socket, reinterpret_cast<const char*>(&setPosition), sizeof(MovePacket), 0);
	}
}

void ClientAcceptSyncPacket::sync_send_packet(array<Client, 3>& clients, void* request_client_ticket) {
	int player_ready_counter = 0;
	for (Client& client : clinets) {
		if (PLAYER_STATE::PLAYER_ACCEPT == clinet.player_state) {
			player_ready_counter += 1;
		}
	}

	if (player_ready_counter == 3) {
		S2CChangeStagePacket packet;
		packet.stageNum = STAGE_TYPE::STAGE_ROLE;
		packet.type = PAKCET_TYPE_S2C::ChangeStage;

		for (Client& clinet : clients) {
			send(client.network_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
		}
	}
	else {
		S2CPlayerPacket packet;
		for (Client& client : clients) {
			if (client.user_ticket == static_cast<int>(request_client_ticket)) {
				packet.type = PACKET_TYPE_S2C::Loading;
				packet.id = static_cast<int>(request_client_ticket);
				send(client.network_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
			}
			else {
				packet.type = PACKET_TYPE_S2C::AddPlayer;
				packet.id = client.user_ticket;
				send(client.network_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
			}
		}
	}
}
