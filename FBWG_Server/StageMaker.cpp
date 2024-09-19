#include "StageMaker.h"

StageMaker::StageMaker()
	: stage_index(STAGE_TYPE::STAGE_ROLE)
{
}

StageMaker::~StageMaker()
{
}

void StageMaker::run_game_stage_thread() {
	
	reset_game_stage();

	while (true) {
		if (STAGE_TYPE::STAGE_ROLE == stage_index) {
			show_stage_role();
		} esle{
			show_stage(stage_index);
		}
	}
}

void StageMaker::show_stage_role()
{
	select_mutex.lock();
	for (Client& client : clients) {
		if (client.have_role()) {
			select_mutex.unlock();
			return;
		}
	}
	select_mutex.unlock();

	show_stage(STAGE_TYPE::STAGE_01);
}

void StageMaker::show_stage(int stage_number)
{
	bool next_stage = true;
	select_mutex.lock();
	for (Client& client : clients) {
		if (stage_index == client.get_curr_stage()) {
			next_stage = false;
			select_mutex.unlock();
			break;
		}
	}
	select_mutex.unlock();

	if (next_stage) {

		reset_game_stage();

		currentJewelyNum = 0;
		isVisibleDoor = false;

		stage_index += 1;

		array<StagePosition, 3> next_stage_positions;
		stage_position.reset_position(stage_index, next_stage_positions);

		_timer.Reset();
		_timer.start_timer();

		for(int client = 0; client < 3; ++client) {
			StageUpdatePacket::send_packet(clients[client].socket, stage_index);

			for (int other_client = 0; other_client < 3; ++other_client) {
				if (client == other_client) {
					continue;
				}
				ClientMovePacket::send_packet(
					clients[client].socoet, next_stage_positions[other_client])
			}
		}
	}
}

void StageMaker::reset_game_stage() {
	ClientAccepter* client_accepter = new ClientAccepter();
	client_accepter->accept_all_client(clients);

	// checking every time jewel count
	// checking every time door open
}



void TimeoutStage()
{
	_timer.Stop();
}

bool StageMaker::check_door()
{
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

void StageMaker::cleanup_game()
{
	WSACleanup();
}