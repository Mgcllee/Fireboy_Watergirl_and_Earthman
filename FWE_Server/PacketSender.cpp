#pragma once

#include "PacketSender.h"

void StageUpdatePacket::send_packet(SOCKET socket, void* stage_index) override {
	S2CChangeStagePacket changePacket;
	changePacket.stageNum = static_cast<int>(stage_index);
	changePacket.type = S2CChangeStage;

	send(socket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
}

void ClientMovePacket::send_packet(SOCKET socket, void* new_position)
{
	StagePosition position = static_cast<StagePosition>(new_position);
	
	MovePacket setPosition;
	setPosition.type = S2CMove_IDLE;
	setPosition.x = position.x;
	setPosition.y = position.y;

	send(socket, reinterpret_cast<char*>(&setPosition), sizeof(MovePacket), 0);
}
