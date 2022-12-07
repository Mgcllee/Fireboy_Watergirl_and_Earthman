#pragma once
#include "stdafx.h"
#include "Stage.h"
#include "Timer.h"
#include<time.h>


array<ThreadInfo, 3> threadHandles;
array<char, 3> playerRole = { 'f', 'f', 'f' };
mutex selectMutex;
array<char, 3> selectPlayerRole = { 'n', 'n', 'n' };
HANDLE multiEvenTthreadHadle[3];
HANDLE jewelyEatHandle;
int stageIndex = -1;

Stage StageMgr;

int currentJewelyNum = 0;	// 먹은 보석 이벤트 핸들 번호
bool isVisibleDoor = false; // 문 비지블 => stage 안에 있으면 상관 없음
mutex jewelyMutex;

DWORD WINAPI ClientWorkThread(LPVOID arg);
DWORD WINAPI ServerWorkThread(LPVOID arg);

void TimeoutStage();
void StageTimerStart();


Timer _timer;
bool isTimeOut = false;
bool gameEnd = false;
double timeoutSeconds = 50;

int main(int argv, char** argc)
{
	wcout.imbue(std::locale("korean"));


	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		Display_Err(WSAGetLastError());
		return 1;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		Display_Err(WSAGetLastError());
		return 1;
	}

	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (::bind(listenSocket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
		Display_Err(WSAGetLastError());
		return 1;
	}

	if (listen(listenSocket, 1) == SOCKET_ERROR) { // 연결할 클라이언트는 1개
		Display_Err(WSAGetLastError());
		return 1;
	}

	for (int i = 0; i < 3; i++)
	{
		SOCKADDR_IN cl_addr;
		int addr_size = sizeof(cl_addr);
		threadHandles[i].clientSocket = accept(listenSocket, reinterpret_cast<sockaddr*>(&cl_addr), &addr_size);
		u_long blockingMode = 1;
		ioctlsocket(threadHandles[i].clientSocket, FIONBIO, &blockingMode);
		int option = TRUE;
		setsockopt(threadHandles[i].clientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));
		if (threadHandles[i].clientSocket == INVALID_SOCKET) {
			Display_Err(WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		cout << "Accept Client[" << i << "]" << endl;
		threadHandles[i].jumpEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
		threadHandles[i].intDoor = CreateEvent(NULL, TRUE, FALSE, NULL);
		ResetEvent(threadHandles[i].jumpEventHandle);
		ResetEvent(threadHandles[i].intDoor);

		S2CPlayerPacket loadPacket;
		loadPacket.type = S2CLoading;
		loadPacket.id = i;
		threadHandles[i].clientId = i;
		threadHandles[i].threadHandle = CreateThread(NULL, 0, ClientWorkThread, reinterpret_cast<LPVOID>(i), 0, NULL);
		multiEvenTthreadHadle[i] = threadHandles[i].threadHandle;
		send(threadHandles[i].clientSocket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);//loading 패킷을 로그인 패킷으로 생각

		loadPacket.type = S2CAddPlayer;
		for (int j = 0; j < 3; j++) {
			if (i != j) {
				if (threadHandles[j].threadHandle != NULL) {
					send(threadHandles[j].clientSocket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);//다른 Player 정보 패킷으로 생각 // j들한테 i의 정보를

					S2CPlayerPacket addPlayerPacket;
					addPlayerPacket.type = S2CAddPlayer;
					addPlayerPacket.id = j;
					send(threadHandles[i].clientSocket, (char*)&addPlayerPacket, sizeof(S2CPlayerPacket), 0);//다른 Player 정보 패킷으로 생각 // i한테 j의 정보를
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
	cout << "All Clients Accpet" << endl;

	cout << "Start server Main Thread" << endl;
	jewelyEatHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(jewelyEatHandle);
	HANDLE serverThread = CreateThread(NULL, 0, ServerWorkThread, reinterpret_cast<LPVOID>(1), 0, NULL);

	while (WSA_WAIT_EVENT_0 + 2 != WSAWaitForMultipleEvents(3, multiEvenTthreadHadle, TRUE, WSA_INFINITE, FALSE)) {}
	for (int j = 0; j < 3; j++) {
		CloseHandle(threadHandles[j].threadHandle);
	}
	CloseHandle(serverThread);


	closesocket(listenSocket);
	WSACleanup();

}

void Display_Err(int Errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, Errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);
	wcout << "ErrorCode: " << Errcode << " - " << (WCHAR*)lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}

void ConstructPacket(ThreadInfo& clientInfo, int ioSize)
{
	int restSize = ioSize + clientInfo.prevSize;
	int needSize = 0;
	char* buf = clientInfo.recvBuf;
	while (restSize != 0) {
		needSize = GetPacketSize(reinterpret_cast<char*>(buf)[0]);
		if (restSize < needSize) {
			clientInfo.prevSize = restSize;
			return;
		}
		else {
			ProcessPacket(clientInfo, reinterpret_cast<char*>(buf));
			memcpy(buf, reinterpret_cast<char*>(buf) + needSize, restSize - needSize);
			restSize -= needSize;
		}
	}
}

DWORD WINAPI ClientWorkThread(LPVOID arg)
{
	int myIndex = reinterpret_cast<int>(arg);
	while (threadHandles[myIndex].clientSocket != INVALID_SOCKET) {
		int recvRetVal = recv(threadHandles[myIndex].clientSocket, threadHandles[myIndex].recvBuf + threadHandles[myIndex].prevSize, MAX_BUF_SIZE - threadHandles[myIndex].prevSize, 0);
		if (recvRetVal > 0) {
			ConstructPacket(threadHandles[myIndex], recvRetVal);
		}
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
			//mutex 넣을 수 도 있어서
			if (jewelyPacket.id != -1)
				for (int j = 0; j < 3; j++)
					send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&jewelyPacket), sizeof(S2CPlayerPacket), 0);

		}

		//
		if (isVisibleDoor) {
			if (StageMgr.door.OBJECT_Collide(threadHandles[myIndex])) {
				DWORD retValDoor = WaitForSingleObject(threadHandles[myIndex].intDoor, 0);
				if (retValDoor != WAIT_OBJECT_0) {
#ifdef DEBUG
					cout << "into Door" << selectPlayerRole[myIndex] << "stage: " << stageIndex << endl;
#endif // DEBUG
					S2CPlayerPacket intoDoorPacket;
					intoDoorPacket.id = myIndex;
					intoDoorPacket.type = S2CIntoDoor;
					SetEvent(threadHandles[myIndex].intDoor);
					for (int j = 0; j < 3; j++)
						send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&intoDoorPacket), sizeof(S2CPlayerPacket), 0);// 문으로 들어가라 명령 => 이거 오면 클라는 문으로 들어가는 애니메이션
				}
			}
		}
	}
	return 0;
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
				// Stage 1 의 정보 획득
				stageIndex = STAGE_01;
				StageMgr.getStage(stageIndex);
				// 최초 위치 설정
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
#ifdef _DEBUG
				cout << "visible Door Off by Next Stage" << endl;
#endif // DEBUG

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
					//스테이지 변경 패킷 전송
					send(threadHandles[i].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);					
					threadHandles[i].ground = 730;
					//스테이지당 최초위치 할당
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
#ifdef  _DEBUG
						cout << "door visible On by jewerly" << endl;
#endif //  _DEBUG
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

					auto startDuration = high_resolution_clock::now() - threadHandles[i].jumpStartTime;		// 저장된 점프 시작부터 경과시간
					auto currentDuration = high_resolution_clock::now() - threadHandles[i].jumpCurrentTime;	// 저정된 점프 현재 시각부터
					MovePacket mPacket;
					mPacket.id = threadHandles[i].clientId;
					mPacket.type = S2CMove_JUMP;

					if (threadHandles[i].Falling || duration_cast<milliseconds>(startDuration).count() > 270) {
						if (threadHandles[i].v < FLT_EPSILON)
							threadHandles[i].v = 0.f;

						if (duration_cast<milliseconds>(currentDuration).count() > 30/* && ((threadHandles[i].y) < threadHandles[i].ground)*/) {//30ms마다 또는 y가 위에 떠 있을때
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

							for (OBJECT& ft : StageMgr.Ft) {// 발판에 안착
								if (ft.Ft_Collision(threadHandles[i]) /*&& (threadHandles[i].y > ft.y - ft.hei * 2)*/) { // 발판 콜라이드와 충돌 확인 && 위에 걸렸다면

									ResetEvent(threadHandles[i].jumpEventHandle); // 점프는 더 이상하지 않음 - 공중에 있지 않는다
									threadHandles[i].direction = DIRECTION::NONE;
									threadHandles[i].v = 0.f;
									threadHandles[i].isJump = false;
									threadHandles[i].Falling = false;
									threadHandles[i].onBoard = ft;
									threadHandles[i].y = threadHandles[i].ground = ft.y - ft.hei; //위치 잡아주기
									mPacket.type = S2CMove_IDLE;
									break;
								}
							}
							mPacket.y = threadHandles[i].y;
							threadHandles[i].jumpCurrentTime = high_resolution_clock::now(); // 다음과 점프시간을 위해 현재 점프한 시간 저장
							for (int j = 0; j < 3; j++) {
								send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&mPacket), sizeof(MovePacket), 0);
							}
						}
					}
					else if (duration_cast<milliseconds>(currentDuration).count() > 30 && !threadHandles[i].Falling) { //상승
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
							if ((ft.y < threadHandles[i].y) && ft.Collision(threadHandles[i])) {//올라가다가 발판에 걸렸다면 떨어져라 머리 충돌
								threadHandles[i].y -= 1.3f * threadHandles[i].v;

								threadHandles[i].v = 0.f;
								threadHandles[i].y += ft.hei;
								threadHandles[i].Falling = true;
								break;
							}
						}

						// 검증된 Y값을 패킷에 담기
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
#ifdef _DEBUG
					cout << "Door Visible True by Time" << endl;
#endif // DEBUG

					isVisibleDoor = true;
				}
			}
		});
}

void TimeoutStage()
{
	_timer.Stop();
}

void ProcessPacket(ThreadInfo& clientInfo, char* packetStart)
{
	if (packetStart == nullptr)
		return;
	switch (reinterpret_cast<char*>(packetStart)[0]) {
	case C2SSelectRole:
	{
		C2SRolePacket* packet = reinterpret_cast<C2SRolePacket*>(packetStart);
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
	break;
	case C2SChangRole:
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
	break;
	case C2SMove:
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
			if(clientInfo.x + 5 >= WINDOW_WID)
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
	break;
	case C2SEndout:
	{
		closesocket(clientInfo.clientSocket);
	}
	break;
	default:
		break;
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
