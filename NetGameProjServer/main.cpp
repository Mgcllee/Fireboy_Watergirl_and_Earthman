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

// 명철 인지 인자들
int currentJewelyNum = 0; // 먹은 보석 이벤트 핸들 번호
bool isVisibleDoor = false; // 문 비지블 => stage 안에 있으면 상관 없음
mutex jewelyMutex;

DWORD WINAPI ClientWorkThread(LPVOID arg);
DWORD WINAPI ServerWorkThread(LPVOID arg);

void TimeoutStage();
void StageTimerStart();


Timer _timer;

double timeoutSeconds = 60 * 5;

int main(int argv, char** argc)
{
	wcout.imbue(std::locale("korean"));


	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		Display_Err(WSAGetLastError());
		return 1;
	}

	//loadFlag= CreateEvent(NULL, FALSE, FALSE, NULL);

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
		if (threadHandles[i].clientSocket == INVALID_SOCKET) {
			Display_Err(WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		cout << "Accept Client[" << i << "]" << endl;
		threadHandles[i].jumpEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
		ResetEvent(threadHandles[i].jumpEventHandle);

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
	while (true) {
		int recvRetVal = recv(threadHandles[myIndex].clientSocket, threadHandles[myIndex].recvBuf + threadHandles[myIndex].prevSize, MAX_BUF_SIZE - threadHandles[myIndex].prevSize, 0);
		if (recvRetVal > 0) {
			ConstructPacket(threadHandles[myIndex], recvRetVal);
		}
		// 명철 인지: 콜라이드 보석, 문
		//여기에 보석 체크? => 부하가 있을까? => 없을거 같긴한데 => 없으니 보석 먹는건 여기에 둡시다.		

		//// 일단 이걸로 ㄱㄱ
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
				S2CPlayerPacket intoDoorPacket;
				intoDoorPacket.id = myIndex;
				intoDoorPacket.type = S2CIntoDoor;
				for (int j = 0; j < 3; j++)
					send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&intoDoorPacket), sizeof(S2CPlayerPacket), 0);// 문으로 들어가라 명령 => 이거 오면 클라는 문으로 들어가는 애니메이션
			}
		}
	}
	return 0;
}

DWORD WINAPI ServerWorkThread(LPVOID arg)
{
	while (true) {
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
				StageMgr.Stage_1();
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
				changePacket.stageNum = STAGE_01;
				changePacket.type = S2CChangeStage;
				for (int x = 0; x < 3; x++) {
					send(threadHandles[x].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
				}
				stageIndex = STAGE_01;
				StageTimerStart();
			}
		}
		else if (stageIndex >= STAGE_01) { // 어떤 인 게임 스테이지가 오든 이건 고정이니까 == -> >= 으로 수정
			DWORD jewelyRetVal = WaitForSingleObject(jewelyEatHandle, 0);
			if (jewelyRetVal == WAIT_OBJECT_0) {
				if (!StageMgr.jewely.empty()) {
					StageMgr.currentVisibleJewely = StageMgr.jewely.front();
					StageMgr.jewely.pop();
					ResetEvent(jewelyEatHandle);
				}
				currentJewelyNum++;
			}
			//명철 인지: 시간 관련은 이한이형이랑 대화해서 해보고
			//보석 먹은 갯수 == StageMgr.MaxJewelyNum && 게임 오버 1분 이상 남았을때 -> 문 보이게 하기 위한

			if (!isVisibleDoor) {
				if (currentJewelyNum == StageMgr.maxJewelyNum) {
					typePacket visibleDoor;
					visibleDoor.type = S2CDoorVisible;
					if (!isVisibleDoor && currentJewelyNum == StageMgr.maxJewelyNum) {// 하지만 시간이 게임 오버까지 1분 이상 남았다면 보석을 다 섭취시 문 위치 보이게 하자
						//isVisibleDoor => 문 비지블 켜주고 => 이거 키면 문 위치를 알고 들어갈 수 있게 하자\					
						for (int i = 0; i < 3; i++)
							send(threadHandles[i].clientSocket, reinterpret_cast<char*>(&visibleDoor), sizeof(typePacket), 0);
						isVisibleDoor = true;
					}
				}
			}


			for (int i = 0; i < 3; i++) {
				if (!threadHandles[i].Falling) {
					if (threadHandles[i].onBoard.FT_Collide_Fall(threadHandles[i])) {
						SetEvent(threadHandles[i].jumpEventHandle);
						threadHandles[i].isJump = true;
						threadHandles[i].Falling = true;
						threadHandles[i].jumpStartTime = high_resolution_clock::now();
						threadHandles[i].jumpCurrentTime = high_resolution_clock::now();
					}
				}

				DWORD retVal = WaitForSingleObject(threadHandles[i].jumpEventHandle, 0);
				if (retVal == WAIT_OBJECT_0) {
					if (!threadHandles[i].isJump) {
						cout << "jump start" << endl;
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

					// 내부 공통 (변수 or for문)은 필요로 넣은 것이니 밖으로 빼지 말아주세용!
					if (threadHandles[i].Falling || duration_cast<milliseconds>(startDuration).count() > 300) {
						if (threadHandles[i].v < FLT_EPSILON)
							threadHandles[i].v = 0.f;

						if (duration_cast<milliseconds>(currentDuration).count() > 30/* && ((threadHandles[i].y) < threadHandles[i].ground)*/) {//30ms마다 또는 y가 위에 떠 있을때
							if (threadHandles[i].direction == DIRECTION::LEFT) {
								threadHandles[i].x -= 10;
							}
							else if (threadHandles[i].direction == DIRECTION::RIGHT) {
								threadHandles[i].x += 10;
							}
							mPacket.x = threadHandles[i].x;
							threadHandles[i].v += threadHandles[i].g;
							threadHandles[i].y += threadHandles[i].v;

							for (OBJECT& ft : StageMgr.Ft) {// 발판에 안착
								if (ft.Ft_Collision(threadHandles[i]) /*&& (threadHandles[i].y > ft.y - ft.hei * 2)*/) { // 발판 콜라이드와 충돌 확인 && 위에 걸렸다면
									cout << "collide on Board" << endl;
									ResetEvent(threadHandles[i].jumpEventHandle); // 점프는 더 이상하지 않음 - 공중에 있지 않는다
									threadHandles[i].direction = DIRECTION::NONE;
									threadHandles[i].v = 0.f;
									threadHandles[i].isJump = false;
									threadHandles[i].Falling = false;
									threadHandles[i].onBoard = ft;
									threadHandles[i].y = threadHandles[i].ground = ft.y - ft.hei; //위치 잡아주기
									cout << "resetEvent: jump" << endl;
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
						//if (threadHandles[i].y > threadHandles[i].ground) {// 캐릭터가 땅에 닿았다면
						//	cout << "resetEvent: jump" << endl;
						//	ResetEvent(threadHandles[i].jumpEventHandle);
						//	threadHandles[i].direction = DIRECTION::NONE;
						//	threadHandles[i].v = 0.f;
						//	threadHandles[i].isJump = false;
						//	threadHandles[i].Falling = false;							
						//	mPacket.x = threadHandles[i].x;
						//	mPacket.y = threadHandles[i].y = threadHandles[i].ground; // 위치 맞춰주기
						//	mPacket.type = S2CMove_IDLE;
						//	for (int j = 0; j < 3; j++) {
						//		send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&mPacket), sizeof(MovePacket), 0);
						//	}
						//}
					}
					else if (duration_cast<milliseconds>(currentDuration).count() > 30 && !threadHandles[i].Falling) { //상승
						if (threadHandles[i].direction == DIRECTION::LEFT) {
							threadHandles[i].x -= 10;
						}
						else if (threadHandles[i].direction == DIRECTION::RIGHT) {
							threadHandles[i].x += 10;
						}
						mPacket.x = threadHandles[i].x;
						threadHandles[i].v -= threadHandles[i].g;
						threadHandles[i].y += threadHandles[i].v;

						for (OBJECT& ft : StageMgr.Ft) {
							if ((ft.y < threadHandles[i].y) && ft.Collision(threadHandles[i])) {//올라가다가 발판에 걸렸다면 떨어져라 머리 충돌
								cout << "collide head" << endl;

								threadHandles[i].v = 0.f;
								threadHandles[i].Falling = true;
								break;
							}
						}
						mPacket.y = threadHandles[i].y += threadHandles[i].v;
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

			if (timeoutSeconds <= packet.timePassed)
			{
				TimeoutStage();
				typePacket timeoutPacket;
				for (int x = 0; x < 3; x++) {
					send(threadHandles[x].clientSocket, (char*)&timeoutPacket, sizeof(typePacket), 0);
				}
			}
			if (packet.timePassed >= 60 * 4) {
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

void TimeoutStage()
{
	_timer.Stop();
}

void ProcessPacket(ThreadInfo& clientInfo, char* packetStart) // 아직 쓰지않는 함수 - recv()하면서 불러줌
{
	if (packetStart == nullptr)
		return;
	switch (reinterpret_cast<char*>(packetStart)[0]) {
	case C2SSelectRole:
	{
		C2SRolePacket* packet = reinterpret_cast<C2SRolePacket*>(packetStart);
		bool change = true;
		//already Exist Role Check

		//non exist Role
		selectMutex.lock();
		for (int i = 0; i < 3; i++) { // 성능이 구릴려나? 상관 없나?
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
		playerRole[clientInfo.clientId] = packet->role; // 캐릭터 둘러보는 것 정도는 상호배제 필요 없다고 생각됨
		//send changePacekt for all Client
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
		MovePacket* packet = reinterpret_cast<MovePacket*>(packetStart);
		DWORD retVal = WaitForSingleObject(clientInfo.jumpEventHandle, 0);
		if (retVal == WAIT_OBJECT_0) {
			return;
		}
		if (packet->y == SHRT_MAX) {
			cout << "setEvent: jump" << endl;
			SetEvent(clientInfo.jumpEventHandle);
			packet->type = S2CMove_JUMP;
		}
		else if (packet->y == SHRT_MIN) {
			clientInfo.direction = DIRECTION::NONE;
			packet->type = S2CMove_IDLE;
		}
		else if (packet->x == 1) {
			if (clientInfo.wid_a <= 10.f)
				clientInfo.wid_a += 0.1f;
			if (clientInfo.wid_v <= 10.f)
				clientInfo.wid_v += clientInfo.wid_a;
			clientInfo.x += clientInfo.wid_v;
			clientInfo.direction = DIRECTION::RIGHT;
			packet->type = S2CMove_RIGHT;
		}
		else if (packet->x == -1) {
			if (clientInfo.wid_a <= 10.f)
				clientInfo.wid_a += 0.1f;
			if (clientInfo.wid_v <= 10.f)
				clientInfo.wid_v += clientInfo.wid_a;
			clientInfo.x -= clientInfo.wid_v;
			clientInfo.direction = DIRECTION::LEFT;
			packet->type = S2CMove_LEFT;
		}
		/*	if (packet->x == 0 && packet->y == 0) {
				clientInfo.wid_v = 0.f;
				clientInfo.wid_a = 0.f;
			}*/

		packet->x = clientInfo.x;
		packet->y = clientInfo.y;

		for (int i = 0; i < 3; i++) {
			send(threadHandles[i].clientSocket, reinterpret_cast<char*>(packet), sizeof(MovePacket), 0);
		}
	}
	break;
	case C2SEndout:
	{
		S2CEndPacket* packet = reinterpret_cast<S2CEndPacket*>(packetStart);
		S2CEndPacket sendPacket;
		sendPacket.type = S2CEndout;

		for (int i = 0; i < 3; i++) {
			send(threadHandles[i].clientSocket, reinterpret_cast<char*>(&sendPacket), sizeof(S2CEndPacket), 0);
		}
	}
	break;

	case C2SRetry:
	{

	}
	break;
	default:
		// Packet Error
		break;
	}
}

int GetPacketSize(char packetType)
{
	int retVal = -1;
	switch (packetType)
	{
	case C2SChangRole:
	case C2SSelectRole:
		retVal = sizeof(C2SRolePacket);
		break;
	case C2SMove:
		retVal = sizeof(MovePacket);
		break;
	case C2SRetry:
	case C2SEndout:
		retVal = sizeof(C2SEndPacket);
		break;
	default:
		break;
	}
	return retVal;
}
