#pragma once

#include "stdafx.h"
#include "protocol.h"

bool NetworkInit(HWND& hWnd, std::string SERVER_ADDR) {
	std::string buf_addr = SERVER_ADDR;

	// 입력받은 문자열에서 모든 온점(.) 제거
	SERVER_ADDR.erase(remove(SERVER_ADDR.begin(), SERVER_ADDR.end(), '.'), SERVER_ADDR.end());

	// 클라이언트 작업용 (서버 연결이 필요할 경우 제거)
	if (SERVER_ADDR.length() == 0) return true;

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

void send_packet(char* buf)
{
	switch (buf[0]) {
	case C2SSelectRole:

		break;
	case C2SChangRole:

		break;
	case C2SMove:

		break;
	case C2SRetry:

		break;
	case C2SExitGame:

		break;
	}
}

void ProcessPacket()
{
	// recv packet

}
