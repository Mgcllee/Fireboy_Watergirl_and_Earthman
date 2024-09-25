#pragma once

#include "PacketSender.h"


PacketSender::PacketSender(array<Client, 3>* member)
	: clients(member)
{
}

void PacketSender::send_packet(Client& client, void* packet)
{
}

void PacketSender::sync_send_packet(void* packet)
{
}

StageUpdatePacket::StageUpdatePacket(array<Client, 3>* member)
	: PacketSender(member)
{
}

void StageUpdatePacket::sync_send_packet(void* stage_index) {
	S2CChangeStagePacket changePacket;
	changePacket.stageNum = *reinterpret_cast<int*>(stage_index);
	changePacket.type = static_cast<int>(PACKET_TYPE_S2C::ChangeStage);

	for (Client& client : *clients) {
		send(client.network_socket, reinterpret_cast<const char*>(&changePacket), sizeof(S2CChangeStagePacket), 0);
	}
}

ClientMovePacket::ClientMovePacket(array<Client, 3>* member)
	: PacketSender(member)
{
}

void ClientMovePacket::sync_send_packet(void* next_position) {
	for (Client& client : *clients) {
		MovePacket setPosition;
		setPosition.type = static_cast<int>(PACKET_TYPE_S2C::Move_IDLE);
		setPosition.x = client.x;
		setPosition.y = client.y;

		for (Client& send_client : *clients) {
			send(send_client.network_socket, reinterpret_cast<const char*>(&setPosition), sizeof(MovePacket), 0);
		}
	}
}

ClientAcceptSyncPacket::ClientAcceptSyncPacket(array<Client, 3>* member)
	: PacketSender(member)
{
}

void ClientAcceptSyncPacket::sync_send_packet(void* request_client_ticket) {
	int player_ready_counter = 0;
	for (Client& client : *clients) {
		if (PLAYER_STATE::PLAYER_ACCEPT == client.player_state
			&& INVALID_SOCKET != client.network_socket) {
			player_ready_counter += 1;
		}
	}

	if (player_ready_counter == 3) {
		S2CChangeStagePacket packet;
		packet.stageNum = STAGE_TYPE::STAGE_ROLE;
		packet.type = static_cast<int>(PACKET_TYPE_S2C::ChangeStage);

		for (Client& client : *clients) {
			send(client.network_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
		}
	}
	else {
		S2CPlayerPacket packet;
		for (Client& client : *clients) {
			int recv_user_ticket = *reinterpret_cast<int*>(request_client_ticket);
			if (client.user_ticket == recv_user_ticket) {
				packet.type = static_cast<int>(PACKET_TYPE_S2C::Loading);
				packet.id = *reinterpret_cast<int*>(request_client_ticket);
				send(client.network_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
			}
			else {
				packet.type = static_cast<char>(PACKET_TYPE_S2C::AddPlayer);
				packet.id = client.user_ticket;
				send(client.network_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
			}
		}
	}
}

StageDoorOpenSyncPacket::StageDoorOpenSyncPacket(array<Client, 3>* member)
	: PacketSender(member)
{
}

void StageDoorOpenSyncPacket::sync_send_packet(void* packet) {
	typePacket send_packet;
	send_packet.type = static_cast<int>(PACKET_TYPE_S2C::DoorVisible);
	for (Client& client : *clients) {
		send(client.network_socket, reinterpret_cast<const char*>(&send_packet), sizeof(send_packet), 0);
	}
}
