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
		MessageBox(hWnd, _T("유효하지 않은 IP 주소입니다."), _T("서버주소 오류!"), NULL);
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
	case S2CLoading:

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

void Display_Err(int Errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, Errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);
	//std::wcout << "ErrorCode: " << Errcode << " - " << (WCHAR*)lpMsgBuf << std::endl;

	//이거 윈도우 그걸로 에러 뜨는거 수정 - 명철 부탁
	LocalFree(lpMsgBuf);
}
