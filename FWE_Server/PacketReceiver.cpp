#include "PacketReceiver.h"

void PacketReceiver::construct_packet(Client& clientInfo, int recv_packet_size) {
	int rest_size = recv_packet_size + clientInfo.rest_packet_size;
	int curr_packet_size = 0;

	char* recv_buufer = reinterpret_cast<char*>(clientInfo.recv_buffer);
	while (rest_size != 0) {
		curr_packet_size = GetPacketSize(recv_buufer[0]);
		if (rest_size < curr_packet_size) {
			clientInfo.rest_packet_size = rest_size;
			return;
		} else {
			ProcessPacket(clientInfo, recv_buufer);
			memcpy(recv_buufer, recv_buufer + curr_packet_size, rest_size - curr_packet_size);
			rest_size -= curr_packet_size;
		}
	}
}

void PacketReceiver::process_packet(Client& client, char* packet)
{
	switch (packet[0]) {
	case PACKET_TYPE_C2S::SelectRole: {
		ClientSelectRole::recv_packet(client, packet);
		break;
		}
	case PACKET_TYPE_C2S::ChangRole: {
		C2SChangRole::recv_packet(client, packet);
		break;
		}
	case PACKET_TYPE_C2S::Move: {
		C2SMove::recv_packet(client, packet);
		break;
		}
	case PACKET_TYPE_C2S::Endout: {
		closesocket(clientInfo.clientSocket);
		break;
		}
	}
}

int GetPacketSize(char packetType)
{
	int retVal = -1;
	int exit = 1;
	switch (packetType)
	{
	case C2SChangRole:
	case C2SSelectRole:
		retVal = sizeof(C2SRolePacket);
		break;
	case C2SMove:
		retVal = sizeof(MovePacket);
		break;
	case C2SEndout:
		retVal = sizeof(typePacket);
		break;
	default:
		break;
	}
	return retVal;
}

void ClientSelectRole::recv_packet(Client& client, void* packet)
{
	C2SRolePacket* packet = reinterpret_cast<C2SRolePacket*>(packet);
	bool change = true;
	selectMutex.lock();
	for (int i = 0; i < 3; i++) {
		if (selectPlayerRole[i] == packet->role) {
			change = false;
			break;
		}
	}
	if (change)
		selectPlayerRole[clientInfo.clientId] = packet->role;
	selectMutex.unlock();
	if (change) {
		//send SelectPacket for all Client
		S2CRolePacket sendPacket;
		sendPacket.id = clientInfo.clientId;
		sendPacket.role = packet->role;
		sendPacket.type = S2CSelectRole;
		for (int i = 0; i < 3; i++) {
			send(threadHandles[i].clientSocket, reinterpret_cast<char*>(&sendPacket), sizeof(S2CRolePacket), 0);
		}
	}
}

void C2SChangRole::recv_packet(Client& client, void* packet)
{
	C2SRolePacket* packet = reinterpret_cast<C2SRolePacket*>(packetStart);
	playerRole[clientInfo.clientId] = packet->role;
	S2CRolePacket sendPacket;
	sendPacket.id = clientInfo.clientId;
	sendPacket.role = packet->role;
	sendPacket.type = S2CChangeRole;
	for (int i = 0; i < 3; i++) {
		send(threadHandles[i].clientSocket, reinterpret_cast<char*>(&sendPacket), sizeof(S2CRolePacket), 0);
	}
}

void C2SMove::recv_packet(Client& client, void* packet)
{
	short prevPosX = clientInfo.x;
	MovePacket* packet = reinterpret_cast<MovePacket*>(packetStart);
	DWORD retVal = WaitForSingleObject(clientInfo.jumpEventHandle, 0);
	if (retVal == WAIT_OBJECT_0) {
		return;
	}
	if (packet->y == SHRT_MAX) {

		SetEvent(clientInfo.jumpEventHandle);
		packet->type = S2CMove_JUMP;
		clientInfo.v = 0.f;
	}
	else if (packet->y == SHRT_MIN) {
		clientInfo.direction = DIRECTION::NONE;
		packet->type = S2CMove_IDLE;
		clientInfo.wid_v = 0;
	}
	else if (packet->x == 1) {
		if (clientInfo.wid_a <= 10.f)
			clientInfo.wid_a += 0.1f;
		if (clientInfo.wid_v <= 10.f)
			clientInfo.wid_v += clientInfo.wid_a;
		ThreadInfo temp = clientInfo;
		temp.x += temp.wid_v;
		for (OBJECT& ft : StageMgr.Ft) {
			if (ft.Ft_Collision(temp) && ft.Ft_Collision(clientInfo)) {
				clientInfo.x -= clientInfo.wid_v;
				clientInfo.wid_v = 0;
				break;
			}
		}
		if (clientInfo.wid_v != 0) {
			clientInfo.x += clientInfo.wid_v;
		}
		if (clientInfo.x + 5 >= WINDOW_WID)
			clientInfo.x = prevPosX;
		clientInfo.direction = DIRECTION::RIGHT;
		packet->type = S2CMove_RIGHT;
	}
	else if (packet->x == -1) {
		if (clientInfo.wid_a <= 10.f)
			clientInfo.wid_a += 0.1f;
		if (clientInfo.wid_v <= 10.f)
			clientInfo.wid_v += clientInfo.wid_a;
		ThreadInfo temp = clientInfo;
		temp.x -= temp.wid_v;
		for (OBJECT& ft : StageMgr.Ft) {
			if (ft.Ft_Collision(temp) && ft.Ft_Collision(clientInfo)) {
				clientInfo.x += clientInfo.wid_v;
				clientInfo.wid_v = 0;
				break;
			}
		}
		if (clientInfo.wid_v != 0) {
			clientInfo.x -= clientInfo.wid_v;
		}
		if (clientInfo.x - 55 < 0)
			clientInfo.x = prevPosX;
		clientInfo.direction = DIRECTION::LEFT;
		packet->type = S2CMove_LEFT;
	}

	packet->x = clientInfo.x;
	packet->y = clientInfo.y;

	for (int i = 0; i < 3; i++) {
		send(threadHandles[i].clientSocket, reinterpret_cast<char*>(packet), sizeof(MovePacket), 0);
	}
}
