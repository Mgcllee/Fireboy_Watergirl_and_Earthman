#pragma once
#include "stdafx.h"
#include "../FWE_Server/protocol.h"
#include "Stage.h"

void Move()
{
	if (stageIndex == 6 || !myCharacterOn) {
		return;
	}
	
	MovePacket move;
	move.type = static_cast<int>(PACKET_TYPE_C2S::Move);
	move.size = sizeof(MovePacket);
	move.id = myId;

	if (keybuffer[VK_UP]) {
		SetEvent(jumpEvent);
		move.y = SHRT_MAX;
	}
	if (keybuffer[VK_LEFT]) {		
		DWORD retVal = WaitForSingleObject(idleStateEvent, 0);
		if (WAIT_OBJECT_0 == retVal)
			ResetEvent(idleStateEvent);		
		move.x = -1;
	}
	else if (keybuffer[VK_RIGHT]) {
		DWORD retVal = WaitForSingleObject(idleStateEvent, 0);
		if (WAIT_OBJECT_0 == retVal)
			ResetEvent(idleStateEvent);		
		move.x = 1;
	}

	if ((!keybuffer[VK_UP] && !keybuffer[VK_RIGHT] && !keybuffer[VK_LEFT])
		|| (keybuffer[VK_RIGHT] && keybuffer[VK_LEFT])) {
		DWORD retVal = WaitForSingleObject(idleStateEvent, 0);
		if (WAIT_OBJECT_0 == retVal)
			return;
		SetEvent(idleStateEvent);
		move.y = SHRT_MIN;
	}

	send(c_socket, reinterpret_cast<char*>(&move), sizeof(move), 0);
	// SendPacket(&move);

}
