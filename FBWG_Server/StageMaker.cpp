#include "StageMaker.h"

StageMaker::StageMaker()
{
}

StageMaker::~StageMaker()
{
}

void StageMaker::make_game_stage() {

	jewelyEatHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(jewelyEatHandle);

}

//HANDLE serverThread = CreateThread(NULL, 0, ServerWorkThread, reinterpret_cast<LPVOID>(1), 0, NULL);
DWORD WINAPI ServerWorkThread(LPVOID arg)
{
	while (threadHandles[0].clientSocket != INVALID_SOCKET && threadHandles[1].clientSocket != INVALID_SOCKET && threadHandles[1].clientSocket != INVALID_SOCKET) {
		if (stageIndex == STAGE_ROLE) {
			bool isFinish = true;
			for (int i = 0; i < 3; i++) {
				if (selectPlayerRole[i] == 'n') {
					isFinish = false;
					break;
				}
			}
			if (isFinish) {
				stageIndex = STAGE_01;
				StageMgr.getStage(stageIndex);
				MovePacket setPosition;
				setPosition.type = S2CMove_IDLE;
				for (int i = 0; i < 3; ++i) {
					setPosition.id = i;
					setPosition.x = threadHandles[i].x;
					setPosition.y = threadHandles[i].y;
					for (int j = 0; j < 3; ++j) {
						threadHandles[j].onBoard = StageMgr.Ground;
						send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&setPosition), sizeof(MovePacket), 0);
					}
				}

				S2CChangeStagePacket changePacket;
				changePacket.stageNum = stageIndex;
				changePacket.type = S2CChangeStage;
				for (int x = 0; x < 3; x++) {
					send(threadHandles[x].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
				}
				StageTimerStart();
			}
		}
		else if (stageIndex >= STAGE_01) {
			bool isNextStage = true;
			for (int i = 0; i < 3; i++) {
				if (threadHandles[i].isArrive) {
					DWORD  retValDoor = WaitForSingleObject(threadHandles[i].intDoor, 0);
					if (retValDoor != WAIT_OBJECT_0) {
						isNextStage = false;
						break;
					}
				}
			}

			if (isNextStage || isTimeOut) {

				currentJewelyNum = 0;
				isVisibleDoor = false;
				if (stageIndex < 6) {
					switch (stageIndex) {
					case STAGE_TITLE:
						stageIndex = STAGE_LOADING;
						break;
					case STAGE_LOADING:
						stageIndex = STAGE_ROLE;
						break;
					case STAGE_ROLE:
						stageIndex = STAGE_01;
						break;
					case STAGE_01:
						stageIndex = STAGE_02;
						break;
					case STAGE_02:
						stageIndex = STAGE_03;
						break;
					case STAGE_03:
						stageIndex = RESULT;
						break;
					}
				}
				StageMgr.getStage(stageIndex);
				ResetEvent(jewelyEatHandle);

				S2CChangeStagePacket changePacket;
				changePacket.stageNum = stageIndex;
				changePacket.type = S2CChangeStage;
				StageTimerStart();

				for (int i = 0; i < 3; i++) {
					ResetEvent(threadHandles[i].intDoor);
					send(threadHandles[i].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
					threadHandles[i].ground = 730;
					MovePacket setPosition;
					setPosition.type = S2CMove_IDLE;
					setPosition.id = i;
					setPosition.x = threadHandles[i].x;
					setPosition.y = threadHandles[i].y;
					for (int j = 0; j < 3; ++j) {
						threadHandles[j].onBoard = StageMgr.Ground;
						send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&setPosition), sizeof(MovePacket), 0);
					}

				}
				isTimeOut = false;
				_timer.Reset();
				isNextStage = false;
			}
			DWORD jewelyRetVal = WaitForSingleObject(jewelyEatHandle, 0);
			if (jewelyRetVal == WAIT_OBJECT_0) {
				if (!StageMgr.jewely.empty()) {
					StageMgr.currentVisibleJewely = StageMgr.jewely.front();
					StageMgr.jewely.pop();
					ResetEvent(jewelyEatHandle);
				}
				currentJewelyNum++;
			}

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
	}
	return 0;
}