#include "Client.h"

Client::Client() {}

Client::~Client() {}

void Client::set_ready_for_play(int user_ticket)
{
	jumpEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	intDoor = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(jumpEventHandle);
	ResetEvent(intDoor);

	S2CPlayerPacket loadPacket;
	loadPacket.type = S2CLoading;
	loadPacket.id = user_ticket;
	clientId = user_ticket;
	threadHandle = CreateThread(NULL, 0, ClientWorkThread, reinterpret_cast<LPVOID>(i), 0, NULL);
	
	multiEvenTthreadHadle[i] = threadHandle;
	
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
