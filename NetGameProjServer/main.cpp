#include<iostream>
#include<WS2tcpip.h>
#include<array>
#include<mutex>
#include<math.h>
#include"protocol.h"

#pragma comment(lib, "ws2_32")

using namespace std;

#define MAX_BUF_SIZE 256

#define STAGE_TITLE			0
#define STAGE_LOADING		1
#define STAGE_ROLE			2
#define STAGE_01			3
#define STAGE_02			4
#define STAGE_03			5

enum PLAYER_STATUS {
	IDLE,
	LEFT,
	RIGHT,
	JUMP,
	LEFT_JUMP,
	RIGHT_JUMP,
	LEFT_DOWN,
	RIGHT_DOWN
};

struct threadInfo {
	HANDLE threadHandle = NULL;
	SOCKET clientSocket;
	char recvBuf[MAX_BUF_SIZE] = { 0 };
	int currentSize;
	int prevSize = 0;
	char clientId = -1;
	short x, y;
	PLAYER_STATUS status = IDLE;

};

void Display_Err(int Errcode);
void ConstructPacket(threadInfo& clientInfo, int ioSize); // 패킷 재조립
void ProcessPacket(threadInfo& clientInfo, char* packetStart); // 패킷 재조립 후, 명령 해석 후 행동
int GetPacketSize(char packetType);

void ChangeRole(); // mutex 필요 없을듯? => change는 딱히 문제 없다고 생각함
void SelectRole(); // mutex 필요 => 두 클라이언트가 동시에 같은 케릭터 선택을 해버리면 안됨
//void MovePacket(); // 움직일 때 마다, 전송
void CheckJewelryEat();// 쥬얼리 습득 확인
void CheckOpenDoor(); // 문 열리는 조건 확인
void PlayerMove(char playerId, short x);


DWORD WINAPI ClientWorkThread(LPVOID arg);
DWORD WINAPI ServerWorkThread(LPVOID arg);

array<threadInfo, 3> threadInfos;
array<char, 3> playerRole = { 'f', 'f', 'f' };
mutex selectMutex;
array<char, 3> selectPlayerRole = { 'f', 'f', 'f' };


HANDLE multiEvenTthreadHadle[3];
HANDLE playerJumpHandle[3] = { NULL }; // JUMP에 대한 핸들값


int stageIndex = -1;

//HANDLE loadFlag; =>웨이트포실

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

	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
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
		threadInfos[i].clientSocket = accept(listenSocket, reinterpret_cast<sockaddr*>(&cl_addr), &addr_size);
		if (threadInfos[i].clientSocket == INVALID_SOCKET) {
			Display_Err(WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		cout << "Accept Client[" << i << "]" << endl;

		S2CPlayerPacket loadPacket;
		loadPacket.type = S2CLoading;
		loadPacket.id = i;
		threadInfos[i].clientId = i;
		threadInfos[i].threadHandle = CreateThread(NULL, 0, ClientWorkThread, reinterpret_cast<LPVOID>(i), 0, NULL);
		multiEvenTthreadHadle[i] = threadInfos[i].threadHandle;
		send(threadInfos[i].clientSocket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);//loading 패킷을 로그인 패킷으로 생각

		loadPacket.type = S2CAddPlayer;
		for (int j = 0; j < 3; j++) {
			if (i != j) {
				if (threadInfos[j].threadHandle != NULL) {
					send(threadInfos[j].clientSocket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);//다른 Player 정보 패킷으로 생각 // j들한테 i의 정보를

					S2CPlayerPacket addPlayerPacket;
					loadPacket.type = S2CAddPlayer;
					loadPacket.id = j;
					send(threadInfos[i].clientSocket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);//다른 Player 정보 패킷으로 생각 // i한테 j의 정보를
				}
			}
		}

		if (i == 2) {
			S2CChangeStagePacket changePacket;

			//무브를 위한 스테이지 변경
			changePacket.stageNum = STAGE_01;
			//changePacket.stageNum = STAGE_ROLE;
			changePacket.type = S2CChangeStage;

			for (int x = 0; x < 3; x++) {
				send(threadInfos[x].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
			}
			stageIndex = STAGE_ROLE;
		}
	}

	for (int i = 0; i < 3; i++) {
		playerJumpHandle[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		ResetEvent(playerJumpHandle[i]);
	}

	HANDLE serverThread = CreateThread(NULL, 0, ServerWorkThread, reinterpret_cast<LPVOID>(1), 0, NULL);

	while (WSA_WAIT_EVENT_0 + 2 != WSAWaitForMultipleEvents(3, multiEvenTthreadHadle, TRUE, WSA_INFINITE, FALSE)) {}
	for (int j = 0; j < 3; j++) {
		CloseHandle(threadInfos[j].threadHandle);
		CloseHandle(playerJumpHandle[j]);
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

void ConstructPacket(threadInfo& clientInfo, int ioSize)
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

void PlayerMove(char playerId, short x)
{
	
	//충돌 체크 해보고
	//떨어진다면
	if (threadInfos[playerId].status == RIGHT) {
		threadInfos[playerId].status = RIGHT_DOWN;
		SetEvent(playerJumpHandle[playerId]);
	}
	else if (threadInfos[playerId].status == LEFT) {
		threadInfos[playerId].status = LEFT_DOWN;
		SetEvent(playerJumpHandle[playerId]);
	}
}

DWORD WINAPI ClientWorkThread(LPVOID arg)
{
	//WaitForSingleObject(loadFlag, INFINITE);

	int myIndex = reinterpret_cast<int>(arg);
	while (true) {
		int recvRetVal = recv(threadInfos[myIndex].clientSocket, threadInfos[myIndex].recvBuf + threadInfos[myIndex].prevSize, MAX_BUF_SIZE - threadInfos[myIndex].prevSize, 0);
		if (recvRetVal != 0) {
			ConstructPacket(threadInfos[myIndex], recvRetVal);
		}
	}
	return 0;
}

DWORD WINAPI ServerWorkThread(LPVOID arg)
{
	//while (true) {
	//	if (stageIndex == STAGE_ROLE) {
	//		bool isFinish = true;
	//		for (int i = 0; i < 3; i++) {
	//			if (selectPlayerRole[i] == 'n') {
	//				isFinish = false;
	//				break;
	//			}
	//		}
	//		if (isFinish) {
	//			//Send All Cleint Next Stage == Stage01
	//			S2CChangeStagePacket changePacket;
	//			changePacket.stageNum = STAGE_01;
	//			changePacket.type = S2CChangeStage;
	//			for (int x = 0; x < 3; x++) {
	//				send(threadInfos[x].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
	//			}
	//			stageIndex = STAGE_01;
	//		}
	//	}
	//}
	while (true) {
		for (int i = 0; i < 3; i++) {
			DWORD ret = WaitForSingleObject(playerJumpHandle[i], 0);
			if (ret == WAIT_OBJECT_0) {
				//만약에 충돌한다면 -> 땅위로 가게 위치 recv 후 ResetEvent();
				//JUMP 모션 send
				//JUMP라면 LEFT RIGHT 확인하고 위치 변경
				//DOWN이라면 ㅇㅇㅇ
			}
		}
	}

	return 0;
}

void ProcessPacket(threadInfo& clientInfo, char* packetStart) // 아직 쓰지않는 함수 - recv()하면서 불러줌
{

	//changePacket() => send S2CChangeRolePacket
	//selectPacket() => mutex Role container and send S2CSelectPacket
	//movePacket(); => 여기서 충돌 체크, 보석 체크 => 여기서 보석을 다 먹었다면 두 클라이언트에게 문 여는 패킷 전송, 문 들어가라는 패킷도 전송해야되네
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
		selectPlayerRole[clientInfo.clientId] = packet->role;
		selectMutex.unlock();
		if (change) {
			//send SelectPacket for all Client
			S2CRolePacket sendPacket;
			sendPacket.id = clientInfo.clientId;
			sendPacket.role = packet->role;
			sendPacket.type = S2CSelectRole;
			for (int i = 0; i < 3; i++) {
				send(threadInfos[i].clientSocket, reinterpret_cast<char*>(&sendPacket), sizeof(S2CRolePacket), 0);
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
			send(threadInfos[i].clientSocket, reinterpret_cast<char*>(&sendPacket), sizeof(S2CRolePacket), 0);
		}
	}
	break;
	case C2SMove:
	{
		MovePacket* packet = reinterpret_cast<MovePacket*>(packetStart);		
		if (packet->y == SHRT_MAX) {
			SetEvent(playerJumpHandle[packet->id]);
		}
		else {
			DWORD ret = WaitForSingleObject(playerJumpHandle[packet->id], 0);
			if (ret == WAIT_FAILED) {
				if (packet->x - threadInfos[packet->id].x > 0) {
					threadInfos[packet->id].status = RIGHT;
				}
				else {
					threadInfos[packet->id].status = LEFT;
				}
				//move();
			}
		}
	}
	break;
	case C2SExitGame:
	{

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
	case S2CLoading:
	case S2CAddPlayer:
		retVal = sizeof(S2CPlayerPacket);
		break;
	case S2CChangeRole:
	case S2CSelectRole:
		retVal = sizeof(S2CRolePacket);
		break;
	case S2CChangeStage:
		retVal = sizeof(S2CChangeStagePacket);
		break;
	case S2CMove:
		retVal = sizeof(MovePacket);
		break;
	case S2CExitGame:
	case S2CDoorOpen:
		retVal = sizeof(typePacket);
		break;
	case S2CJewelryVisibility:
		retVal = sizeof(S2CJewelryVisibilityPacket);
		break;
	default:
		break;
	}
	return retVal;
}