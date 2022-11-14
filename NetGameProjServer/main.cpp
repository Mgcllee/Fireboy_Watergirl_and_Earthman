#include<iostream>
#include<WS2tcpip.h>
#include<array>
#include<mutex>
#include"protocol.h"

#pragma comment(lib, "ws2_32")

using namespace std;

#define MAX_BUF_SIZE 256

void Display_Err(int Errcode);
void ConstructPacket(char* recvPacket); // 패킷 재조립
void ProcessPacket(char* packetStart); // 패킷 재조립 후, 명령 해석 후 행동
void ChangeRole(); // mutex 필요 없을듯? => change는 딱히 문제 없다고 생각함
void SelectRole(); // mutex 필요 => 두 클라이언트가 동시에 같은 케릭터 선택을 해버리면 안됨
void MovePacket(); // 움직일 때 마다, 전송
void CheckJewelryEat();// 쥬얼리 습득 확인
void CheckOpenDoor(); // 문 열리는 조건 확인




DWORD WINAPI ClientWorkThread(LPVOID arg);

struct threadInfo {
	HANDLE h = NULL;
	SOCKET clientSocket;
	char recvBuf[MAX_BUF_SIZE] = { 0 };
	int currentSize;
	int prev_size = 0;
};

array<threadInfo, 3> threadHandles;

//HANDLE loadFlag; =>웨이트포실긍

//map<socket, Role> 하는게 나을듯?

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

	array<SOCKET, 3> clientSocket;

	for (int i = 0; i < 3; i++)
	{
		SOCKADDR_IN cl_addr;
		int addr_size = sizeof(cl_addr);
		threadHandles[i].clientSocket = accept(listenSocket, reinterpret_cast<sockaddr*>(&cl_addr), &addr_size);
		if (clientSocket[i] == INVALID_SOCKET) {
			Display_Err(WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		cout << "Accept Client[" << i <<"]" << endl;

		S2CPlayerPacket load1;
		load1.type = S2CLoading;
		load1.id = i;

		threadHandles[i].h = CreateThread(NULL, 0, ClientWorkThread, reinterpret_cast<LPVOID>(i), 0, NULL);

		send(threadHandles[i].clientSocket, (char*)&load1, sizeof(S2CPlayerPacket), 0);//loading 패킷을 로그인 패킷으로 생각하고
		
		
		if (i == 2) {
			S2CChangeStagePacket change1;
			change1.stageNum = 0;
			change1.type = S2CChangeStage;

			for (int x = 0; x < 3; x++)
				send(clientSocket[i], (char*)&change1, sizeof(S2CChangeStage), 0);
		}
	}


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

void ConstructPacket(char* recvPacket)
{

}

DWORD WINAPI ClientWorkThread(LPVOID arg)
{
	//WaitForSingleObject(loadFlag, INFINITE);

	int myIndex = reinterpret_cast<int>(arg);
	while (true) {
		int recvRetVal = recv(threadHandles[myIndex].clientSocket, threadHandles[myIndex].recvBuf + threadHandles[myIndex].prev_size, MAX_BUF_SIZE - threadHandles[myIndex].prev_size, 0);
		if (!recvRetVal) {
			ConstructPacket(threadHandles[myIndex].recvBuf + threadHandles[myIndex].prev_size);
		}
	}
	return 0;
}

void ProcessPacket(char* packetStart) // 아직 쓰지않는 함수 - recv()하면서 불러줌
{

	//changePacket() => send S2CChangeRolePacket
	//selectPacket() => mutex Role container and send S2CSelectPacket
	//movePacket(); => 여기서 충돌 체크, 보석 체크 => 여기서 보석을 다 먹었다면 두 클라이언트에게 문 여는 패킷 전송, 문 들어가라는 패킷도 전송해야되네
}
