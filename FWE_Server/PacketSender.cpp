#pragma once

#include "PacketSender.h"

void StageUpdatePacket::send_packet(Clinet& client, void* stage_index) override {
	S2CChangeStagePacket changePacket;
	changePacket.stageNum = static_cast<int>(stage_index);
	changePacket.type = PACKET_TYPE_S2C::ChangeStage;

	send(client.socket, reinterpret_cast<char*>(&changePacket), sizeof(S2CChangeStagePacket), 0);
}

void ClientMovePacket::send_packet(Clinet& client, void* new_position)
{
	StagePosition position = static_cast<StagePosition>(new_position);
	
	MovePacket setPosition;
	setPosition.type = PACKET_TYPE_S2C::Move_IDLE;
	setPosition.x = position.x;
	setPosition.y = position.y;

	send(client.socket, reinterpret_cast<char*>(&setPosition), sizeof(MovePacket), 0);
}

void ClientServerIDPacket::send_packet(Client& clinet, void* user_ticket)
{
	S2CPlayerPacket loadPacket;
	loadPacket.type = PACKET_TYPE_S2C::Loading;
	loadPacket.id = user_ticket;
	clientId = user_ticket;

	// threadHandle = CreateThread(NULL, 0, ClientWorkThread, reinterpret_cast<LPVOID>(i), 0, NULL);
	// multiEvenTthreadHadle[i] = threadHandle;

	send(socket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);

	loadPacket.type = S2CAddPlayer;
	for (int j = 0; j < 3; j++) {
		if (i != j) {
			if (threadHandles[j].threadHandle != NULL) {
				send(threadHandles[j].clientSocket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);

				S2CPlayerPacket addPlayerPacket;
				addPlayerPacket.type = S2CAddPlayer;
				addPlayerPacket.id = j;
				send(clientSocket, (char*)&addPlayerPacket, sizeof(S2CPlayerPacket), 0);
			}
		}
	}

	if (i == 2) {
		S2CChangeStagePacket changePacket;
		changePacket.stageNum = STAGE_ROLE;
		changePacket.type = S2CChangeStage;

		for (int x = 0; x < 3; x++) {
			send(threadHandles[x].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
		}
		stageIndex = STAGE_ROLE;
	}
}
