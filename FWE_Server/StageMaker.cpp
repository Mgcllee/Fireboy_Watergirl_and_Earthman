#include "StageMaker.h"

StageMaker::StageMaker()
	: stage_index(STAGE_TYPE::STAGE_TITLE)
{
}

void StageMaker::run_game_stage_thread() {
	client_accepter.accept_all_client(clients);
	
	stage_index = STAGE_TYPE::STAGE_ROLE;
	show_character_select_stage();

	while (true) {
		// TODO: add [exit condition]

		if (check_next_stage_condition()) {
			show_game_stage(stage_index);
			reset_game_stage();
		}

		check_jewely();
		check_door();
	}
}

void StageMaker::show_character_select_stage() {
	select_mutex.lock();
	for (Client& client : clients) {
		if (client.have_role()) {
			select_mutex.unlock();
			return;
		}
	}
	select_mutex.unlock();

	stage_index = STAGE_TYPE::STAGE_01;
	show_game_stage(stage_index);
}

void StageMaker::show_game_stage(int stage_number) {
	if (check_next_stage_condition()) {

		reset_game_stage();

		if (stage_index != STAGE_TYPE::RESULT) {
			stage_index += 1;
		}

		StageUpdatePacket::sync_send_packet(clients, stage_index);
		
		array<StagePosition, 3> next_stage_positions;
		stage_position.reset_position(stage_index, next_stage_positions);
		for (StagePosition position : next_stage_positions) {
			ClientMovePacket::sync_send_packet(clients, position);
		}
	}
}

void StageMaker::reset_game_stage() {
	_timer.Reset();
	_timer.start_timer();

	// checking every time jewel count
	// checking every time door open
}

bool StageMaker::check_door() {
	if (!isVisibleDoor) {
		if (currentJewelyNum == StageMgr.maxJewelyNum) {
			typePacket visibleDoor;
			visibleDoor.type = S2CDoorVisible;
			if (!isVisibleDoor && currentJewelyNum == StageMgr.maxJewelyNum) {
				for (int i = 0; i < 3; i++)
					send(threadHandles[i].clientSocket, reinterpret_cast<char*>(&visibleDoor), sizeof(typePacket), 0);
				isVisibleDoor = true;
			}
		}
	}
	return false;
}

void refactoring_board() {


	if (StageMgr.currentVisibleJewely.OBJECT_Collide(threadHandles[myIndex])) {
		S2CPlayerPacket jewelyPacket;
		jewelyPacket.id = -1;
		jewelyPacket.type = S2CEatJewely;
		DWORD retVal = WaitForSingleObject(jewelyEatHandle, 0);
		if (retVal != WAIT_OBJECT_0) {
			threadHandles[myIndex].score++;
			SetEvent(jewelyEatHandle);
			jewelyPacket.id = myIndex;
		}
		if (jewelyPacket.id != -1)
			for (int j = 0; j < 3; j++)
				send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&jewelyPacket), sizeof(S2CPlayerPacket), 0);

	}

	if (isVisibleDoor) {
		if (StageMgr.door.OBJECT_Collide(threadHandles[myIndex])) {
			DWORD retValDoor = WaitForSingleObject(threadHandles[myIndex].intDoor, 0);
			if (retValDoor != WAIT_OBJECT_0) {
				S2CPlayerPacket intoDoorPacket;
				intoDoorPacket.id = myIndex;
				intoDoorPacket.type = S2CIntoDoor;
				SetEvent(threadHandles[myIndex].intDoor);
				for (int j = 0; j < 3; j++)
					send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&intoDoorPacket), sizeof(S2CPlayerPacket), 0);
			}
		}
	}


}

bool StageMaker::check_jewely()
{
	DWORD jewelyRetVal = WaitForSingleObject(jewelyEatHandle, 0);
	if (jewelyRetVal == WAIT_OBJECT_0) {
		if (!stage.jewely.empty()) {
			StageMgr.currentVisibleJewely = StageMgr.jewely.front();
			StageMgr.jewely.pop();
			ResetEvent(jewelyEatHandle);
		}
		currentJewelyNum++;
	}

	return false;
}

void StageMaker::move_interpolation()
{
	for (int i = 0; i < 3; i++) {
		if (!threadHandles[i].Falling) {
			if (threadHandles[i].onBoard.FT_Collide_Fall(threadHandles[i])) {
				DWORD retVal = WaitForSingleObject(threadHandles[i].jumpEventHandle, 0);
				if (retVal != WAIT_OBJECT_0) {
					SetEvent(threadHandles[i].jumpEventHandle);
					threadHandles[i].isJump = true;
					threadHandles[i].Falling = true;
					threadHandles[i].jumpStartTime = high_resolution_clock::now();
					threadHandles[i].jumpCurrentTime = high_resolution_clock::now();
				}
			}
		}

		DWORD retVal = WaitForSingleObject(threadHandles[i].jumpEventHandle, 0);
		if (retVal == WAIT_OBJECT_0) {
			if (!threadHandles[i].isJump) {
				threadHandles[i].isJump = true;
				threadHandles[i].jumpStartTime = high_resolution_clock::now();
				threadHandles[i].jumpCurrentTime = high_resolution_clock::now();
				threadHandles[i].v = 0.f;
				threadHandles[i].y = threadHandles[i].ground;
			}

			auto startDuration = high_resolution_clock::now() - threadHandles[i].jumpStartTime;
			auto currentDuration = high_resolution_clock::now() - threadHandles[i].jumpCurrentTime;
			MovePacket mPacket;
			mPacket.id = threadHandles[i].clientId;
			mPacket.type = S2CMove_JUMP;

			if (threadHandles[i].Falling || duration_cast<milliseconds>(startDuration).count() > 270) {
				if (threadHandles[i].v < FLT_EPSILON)
					threadHandles[i].v = 0.f;

				if (duration_cast<milliseconds>(currentDuration).count() > 30) {
					int prevPosX = threadHandles[i].x;
					if (threadHandles[i].direction == DIRECTION::LEFT) {
						if ((threadHandles[i].x - threadHandles[i].wid_v < WINDOW_WID - threadHandles[i].wid)
							&& (threadHandles[i].x - threadHandles[i].wid_v > threadHandles[i].wid))
							threadHandles[i].x -= threadHandles[i].wid_v;
						else
							threadHandles[i].x += threadHandles[i].wid_v;
					}
					else if (threadHandles[i].direction == DIRECTION::RIGHT) {
						if ((threadHandles[i].x + threadHandles[i].wid_v < WINDOW_WID - threadHandles[i].wid / 2)
							&& (threadHandles[i].x + threadHandles[i].wid_v > threadHandles[i].wid))
							threadHandles[i].x += threadHandles[i].wid_v;
						else
							threadHandles[i].x -= threadHandles[i].wid_v;
					}

					if (threadHandles[i].x + 5 >= WINDOW_WID)
						threadHandles[i].x = prevPosX;
					if (threadHandles[i].x - 55 < 0)
						threadHandles[i].x = prevPosX;
					mPacket.x = threadHandles[i].x;

					mPacket.x = threadHandles[i].x;
					threadHandles[i].v += threadHandles[i].g;
					threadHandles[i].y += threadHandles[i].v;

					for (OBJECT& ft : StageMgr.Ft) {
						if (ft.Ft_Collision(threadHandles[i])) {

							ResetEvent(threadHandles[i].jumpEventHandle);
							threadHandles[i].direction = DIRECTION::NONE;
							threadHandles[i].v = 0.f;
							threadHandles[i].isJump = false;
							threadHandles[i].Falling = false;
							threadHandles[i].onBoard = ft;
							threadHandles[i].y = threadHandles[i].ground = ft.y - ft.hei;
							mPacket.type = S2CMove_IDLE;
							break;
						}
					}
					mPacket.y = threadHandles[i].y;
					threadHandles[i].jumpCurrentTime = high_resolution_clock::now();
					for (int j = 0; j < 3; j++) {
						send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&mPacket), sizeof(MovePacket), 0);
					}
				}
			}
			else if (duration_cast<milliseconds>(currentDuration).count() > 30 && !threadHandles[i].Falling) {
				int prevPosX = threadHandles[i].x;
				if (threadHandles[i].direction == DIRECTION::LEFT) {
					if ((threadHandles[i].x - threadHandles[i].wid_v < WINDOW_WID - threadHandles[i].wid)
						&& (threadHandles[i].x - threadHandles[i].wid_v > threadHandles[i].wid))
						threadHandles[i].x -= threadHandles[i].wid_v;
					else
						threadHandles[i].x += threadHandles[i].wid_v;
				}
				else if (threadHandles[i].direction == DIRECTION::RIGHT) {
					if ((threadHandles[i].x + threadHandles[i].wid_v < WINDOW_WID - threadHandles[i].wid / 2)
						&& (threadHandles[i].x + threadHandles[i].wid_v > threadHandles[i].wid))
						threadHandles[i].x += threadHandles[i].wid_v;
					else
						threadHandles[i].x -= threadHandles[i].wid_v;
				}

				if (threadHandles[i].x + 5 >= WINDOW_WID)
					threadHandles[i].x = prevPosX;
				if (threadHandles[i].x - 55 < 0)
					threadHandles[i].x = prevPosX;
				mPacket.x = threadHandles[i].x;

				threadHandles[i].v -= threadHandles[i].g;
				threadHandles[i].y += 1.3f * threadHandles[i].v;

				for (OBJECT& ft : StageMgr.Ft) {
					if ((ft.y < threadHandles[i].y) && ft.Collision(threadHandles[i])) {
						threadHandles[i].y -= 1.3f * threadHandles[i].v;

						threadHandles[i].v = 0.f;
						threadHandles[i].y += ft.hei;
						threadHandles[i].Falling = true;
						break;
					}
				}

				mPacket.y = threadHandles[i].y;
				threadHandles[i].jumpCurrentTime = high_resolution_clock::now();
				for (int j = 0; j < 3; j++) {
					send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&mPacket), sizeof(MovePacket), 0);
				}
			}
		}
	}
}

void StageMaker::cleanup_game() {
	for (Client& client : clients) {
		closesocket(client.socket);
	}
	WSACleanup();
}

bool StageMaker::check_next_stage_condition() {
	select_mutex.lock();
	for (Client& client : clients) {
		if (stage_index == client.get_curr_stage()) {
			return false;
			select_mutex.unlock();
			break;
		}
	}
	select_mutex.unlock();
	return true;
}
