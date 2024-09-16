#include "StageMaker.h"

StageMaker::StageMaker()
{
}

StageMaker::~StageMaker()
{
}

void TimeoutStage()
{
	_timer.Stop();
}


void StageTimerStart()
{
	if (_timer.IsRunning() == true)
	{
		return;
	}

	_timer.Start(std::chrono::milliseconds(1000), [=]
		{
			S2CStageTimePassPacket packet;
			packet.timePassed = _timer.GetElapsedTime() / (double)1000;

			for (int x = 0; x < 3; x++) {
				send(threadHandles[x].clientSocket, (char*)&packet, sizeof(S2CStageTimePassPacket), 0);
			}

			if (timeoutSeconds <= packet.timePassed && !isTimeOut && !gameEnd)
			{
				typePacket timeoutPacket;
				for (int x = 0; x < 3; x++) {
					send(threadHandles[x].clientSocket, (char*)&timeoutPacket, sizeof(typePacket), 0);
				}
				DWORD retValDoor0 = WaitForSingleObject(threadHandles[0].intDoor, 0);
				DWORD retValDoor1 = WaitForSingleObject(threadHandles[1].intDoor, 0);
				DWORD retValDoor2 = WaitForSingleObject(threadHandles[2].intDoor, 0);

				if (retValDoor0 != WAIT_OBJECT_0 && retValDoor1 != WAIT_OBJECT_0 && retValDoor2 != WAIT_OBJECT_0) {
					S2CChangeStagePacket changePacket;
					changePacket.stageNum = RESULT;
					changePacket.type = S2CChangeStage;
					for (int x = 0; x < 3; x++) {
						send(threadHandles[x].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
					}
					isTimeOut = false;
					gameEnd = true;
					TimeoutStage();
				}
				else {
					if (retValDoor0 != WAIT_OBJECT_0 && threadHandles[0].isArrive) {
						S2CPlayerPacket playerPacket;
						playerPacket.id = threadHandles[0].clientId;
						cout << "out Player: " << (int)playerPacket.id << endl;
						playerPacket.type = S2CPlayerOut;
						threadHandles[0].isArrive = false;
						for (int x = 0; x < 3; x++) {
							send(threadHandles[x].clientSocket, (char*)&playerPacket, sizeof(S2CPlayerPacket), 0);
						}
					}
					if (retValDoor1 != WAIT_OBJECT_0 && threadHandles[1].isArrive) {
						S2CPlayerPacket playerPacket;
						playerPacket.id = threadHandles[1].clientId;
						cout << "out Player: " << (int)playerPacket.id << endl;
						playerPacket.type = S2CPlayerOut;
						threadHandles[1].isArrive = false;
						for (int x = 0; x < 3; x++) {
							send(threadHandles[x].clientSocket, (char*)&playerPacket, sizeof(S2CPlayerPacket), 0);
						}
					}
					if (retValDoor2 != WAIT_OBJECT_0 && threadHandles[2].isArrive) {
						S2CPlayerPacket playerPacket;
						playerPacket.id = threadHandles[2].clientId;
						cout << "out Player: " << (int)playerPacket.id << endl;
						playerPacket.type = S2CPlayerOut;
						threadHandles[2].isArrive = false;
						for (int x = 0; x < 3; x++) {
							send(threadHandles[x].clientSocket, (char*)&playerPacket, sizeof(S2CPlayerPacket), 0);
						}
					}
					isTimeOut = true;
				}

			}
			if (packet.timePassed >= 35 && !isTimeOut && !gameEnd) {
				if (!isVisibleDoor) {
					typePacket visibleDoorPacket;
					visibleDoorPacket.type = S2CDoorVisible;
					for (int x = 0; x < 3; x++) {
						send(threadHandles[x].clientSocket, (char*)&visibleDoorPacket, sizeof(typePacket), 0);
					}
					isVisibleDoor = true;
				}
			}
		});
}

void StageMaker::make_game_stage() {
	ClientAccepter* client_accepter = new ClientAccepter();
	client_accepter->accept_all_client(clients);

	jewelyEatHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(jewelyEatHandle);

	
}

void StageMaker::cleanup_game()
{
	WSACleanup();
}

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