#pragma once

#include "stdafx.h"
#include "protocol.h"

bool NetworkInit(HWND& hWnd, std::string SERVER_ADDR) {
	// 클라이언트 작업용 (서버 연결이 필요할 경우 제거)
	if (SERVER_ADDR.length() == 0) return true;

	std::string buf_addr = SERVER_ADDR;

	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	inet_pton(AF_INET, SERVER_ADDR.c_str(), &server_addr.sin_addr);


	if (connect(c_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
		int aa = WSAGetLastError();
		if (aa != 0) {
			MessageBox(hWnd, _T("유효하지 않은 IP 주소입니다."), _T("서버주소 오류!"), NULL);
			Display_Err(hWnd, WSAGetLastError());
			return false;
		}
	}

	u_long nonBlock = 1;
	if (ioctlsocket(c_socket, FIONBIO, &nonBlock) == INVALID_SOCKET) {
		return false;
	}

	// 클라이언트 윈도우 창 이름 변경
	SetWindowTextA(hWnd, (buf_addr + " 주소의 서버에 접속 성공!").c_str());

	// Network Todo


	return true;
}

void ProcessPacket(char* buf)
{
	if (buf == nullptr)
		return;

	switch (reinterpret_cast<char*>(buf)[0]) {
	case S2CLoading:
	{
		S2CPlayerPacket* packet = reinterpret_cast<S2CPlayerPacket*>(buf);
		myId = packet->id;
		players[0].id = myId;
	}
	break;
	case S2CAddPlayer:
	{
		S2CPlayerPacket* packet = reinterpret_cast<S2CPlayerPacket*>(buf);
		myId = packet->id;
		for (int i = 1; i < 3; i++)
			if (players[i].id == -1)
				players[i].id = myId;
		currneClientNum++;
	}
	break;
	case S2CSelectRole:

		break;
	case S2CChangeRole:

		break;
	case S2CMove:
	{
		MovePacket* move = reinterpret_cast<MovePacket*>(buf);
		players[move->id].x = move->x;
		players[move->id].y = move->y;
	}
	break;
	case S2CDoorOpen:

		break;
	case S2CExitGame:

		break;
	case S2CJewelryVisibility:

		break;
	case S2CChangeStage:
	{
		S2CChangeStagePacket* packet = reinterpret_cast<S2CChangeStagePacket*>(buf);
		stageIndex =  packet->stageNum;
	}
		break;
	default:
		// Packet Error
		break;
	}
}

void SendPacket(void* buf)
{
	if (buf == nullptr)
		return;
	char* packet = nullptr;
	int size = 0;
	switch (reinterpret_cast<char*>(buf)[0]) {
	case C2SSelectRole:
		size = sizeof(C2SRolePacket);
		packet = new char[size];
		memcpy(packet, buf, size);
		break;
	case C2SChangeRole:

		break;
	case C2SMove:
		size = sizeof(MovePacket);
		packet = new char[size];
		memcpy(packet, buf, size);
		break;
	case C2SRetry:

		break;
	case C2SExitGame:

		break;
	
	default:
		// Packet Error
		break;
	}

	if (packet != nullptr) {
		send(c_socket, packet, size, NULL);
		delete[] packet;
	}
}

void ConstructPacket(void* buf, int ioSize)
{
	int restSize = ioSize + prevSize;
	int needSize = 0;
	while (restSize != 0) {
		needSize = GetPacketSize(reinterpret_cast<char*>(buf)[0]);
		if (restSize < needSize) {
			prevSize = restSize;
			return;
		}
		else {
			ProcessPacket(reinterpret_cast<char*>(buf));
			memcpy(buf, reinterpret_cast<char*>(buf) + needSize, restSize - needSize);
			restSize -= needSize;
		}
	}
}

void Display_Err(HWND hWnd, int Errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, Errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);
	//std::wcout << "ErrorCode: " << Errcode << " - " << (WCHAR*)lpMsgBuf << std::endl;
	MessageBox(hWnd, (LPWSTR)lpMsgBuf, _T("서버주소 오류!"), NULL);
	//이거 윈도우 그걸로 에러 뜨는거 수정 - 명철 부탁
	LocalFree(lpMsgBuf);
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