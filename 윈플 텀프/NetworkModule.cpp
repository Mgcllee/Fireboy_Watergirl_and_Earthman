#pragma once
#include<WS2tcpip.h>
#include "stdafx.h"
#include "protocol.h"
#include "StageMgr.h"

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
		SetEvent(changeStageEvent);
	}
	break;
	case S2CAddPlayer:
	{
		S2CPlayerPacket* packet = reinterpret_cast<S2CPlayerPacket*>(buf);
		for (int i = 1; i < 3; i++)
			if (players[i].id == -1) {
				players[i].id = packet->id;
				break;
			}
		currneClientNum++;
	}
	break;
	case S2CSelectRole:
	{
		S2CRolePacket* packet = reinterpret_cast<S2CRolePacket*>(buf);
		for (int i = 0; i < 3; i++)
			if (players[i].id == packet->id) {
				if (packet->id == myId)
					SetEvent(selectMyCharacter);
				players[i].role = packet->role;
				if (players[i].role == 'f') {
					players[i].Anim[0].Load(L"Resource\\빨강 정지 215.411.png");
					players[i].Anim[1].Load(L"Resource\\빨강 정지 215.411.png");
					players[i].Anim[2].Load(L"Resource\\빨강 오른쪽 342.271.png");
					players[i].Anim[3].Load(L"Resource\\빨강 하강 215.411.png");
					players[i].Anim[4].Load(L"Resource\\빨강 왼쪽 342.271.png");

					players[i].C_img_X_Size_01 = 215;
					players[i].C_img_Y_Size_01 = 411;
					players[i].C_img_Frame = 342;
					players[i].C_img_X_Size_02 = 342;
					players[i].C_img_Y_Size_02 = 271;
				}
				else if (players[i].role == 'w') {
					players[i].Anim[0].Load(L"Resource\\파랑 정지 215.411.png");
					players[i].Anim[1].Load(L"Resource\\파랑 정지 215.411.png");
					players[i].Anim[2].Load(L"Resource\\파랑 오른쪽 376.480.png");
					players[i].Anim[3].Load(L"Resource\\파랑 하강 215.411.png");
					players[i].Anim[4].Load(L"Resource\\파랑 왼쪽 376.480.png");
					players[i].C_img_X_Size_01 = 215;
					players[i].C_img_Y_Size_01 = 411;
					players[i].C_img_Frame = 376;
					players[i].C_img_X_Size_02 = 376;
					players[i].C_img_Y_Size_02 = 480;
				}
				else if (players[i].role == 'e') {
					players[i].Anim[0].Load(L"Resource\\흙_정지_215.411.png");
					players[i].Anim[1].Load(L"Resource\\흙_정지_215.411.png");
					players[i].Anim[2].Load(L"Resource\\흙_오른쪽_376.480.png");
					players[i].Anim[3].Load(L"Resource\\흙_하강_215.411.png");
					players[i].Anim[4].Load(L"Resource\\흙_왼쪽_376.480.png");
					players[i].C_img_X_Size_01 = 215;
					players[i].C_img_Y_Size_01 = 411;
					players[i].C_img_Frame = 376;
					players[i].C_img_X_Size_02 = 376;
					players[i].C_img_Y_Size_02 = 480;
				}
				break;
			}

	}
	break;
	case S2CChangeRole:
	{
		S2CRolePacket* packet = reinterpret_cast<S2CRolePacket*>(buf);
		for (int i = 0; i < 3; i++)
			if (players[i].id == packet->id) {
				players[i].role = packet->role;
				break;
			}
	}
	break;
	case S2CMove_IDLE:
	{
		MovePacket* packet = reinterpret_cast<MovePacket*>(buf);
		for (int i = 0; i < 3; ++i) {
			if (players[i].id == packet->id) {
				players[i].direction = DIRECTION::IDLE;
				players[i].x = packet->x;
				players[i].y = packet->y;
				break;
			}
		}
		if (packet->id == myId) {
			DWORD retVal = WaitForSingleObject(jumpEvent, 0);
			if (retVal == WAIT_OBJECT_0)
				ResetEvent(jumpEvent);
		}
	}
	break;
	case S2CMove_JUMP:
	{
		MovePacket* packet = reinterpret_cast<MovePacket*>(buf);
		for (int i = 0; i < 3; ++i) {
			if (players[i].id == packet->id) {
				players[i].direction = DIRECTION::JUMP;
				players[i].x = packet->x;
				players[i].y = packet->y;
				break;
			}
		}
	}
	break;
	case S2CMove_LEFT:
	{
		MovePacket* packet = reinterpret_cast<MovePacket*>(buf);
		for (int i = 0; i < 3; ++i) {
			if (players[i].id == packet->id) {
				players[i].direction = DIRECTION::LEFT;
				players[i].x = packet->x;
				players[i].y = packet->y;
				break;
			}
		}
	}
	break;
	case S2CMove_RIGHT:
	{
		MovePacket* packet = reinterpret_cast<MovePacket*>(buf);
		for (int i = 0; i < 3; ++i) {
			if (players[i].id == packet->id) {
				players[i].direction = DIRECTION::RIGHT;
				players[i].x = packet->x;
				players[i].y = packet->y;
				break;
			}
		}
	}
	break;
	case S2CEndout:
	{
		C2SEndPacket* packet = reinterpret_cast<C2SEndPacket*>(buf);
		exit(1);
		break;
	}
	case S2CExitGame:

		break;
		/*case S2CJewelryVisibility:

			break;*/
	case S2CChangeStage:
	{
		S2CChangeStagePacket* packet = reinterpret_cast<S2CChangeStagePacket*>(buf);
		stageIndex = packet->stageNum;
		SetEvent(changeStageEvent);
		StageMgr::StageTimepass = 0;
		break;
		StageMgr::IsTimeoutStageEnd = false;
	}
	case S2CStageTimePass:
	{
		S2CStageTimePassPacket* packet = reinterpret_cast<S2CStageTimePassPacket*>(buf);
		StageMgr::StageTimepass = packet->timePassed;

	}
	break;
	case S2CStageTimeout:
	{
		typePacket* packet = reinterpret_cast<typePacket*>(buf);
		StageMgr::IsTimeoutStageEnd = true;
	}
	break;
	case S2CDoorVisible:
		doorVisible = true;
		break;
	case S2CIntoDoor:
	{
		//Client Into Door = true; => PlayerInfo에서 bool넣어서 판단하고 애니메이션 해서 비지블 관리
		S2CPlayerPacket* packet = reinterpret_cast<S2CPlayerPacket*>(buf);

		for (int i = 0; i < 3; i++) {
			if (players[i].id == packet->id) {
				players[i].isIntoDoor = true;
				players[i].Frame = 0;
			}
		}
	}
	break;
	case S2CEatJewely:
	{
		S2CPlayerPacket* packet = reinterpret_cast<S2CPlayerPacket*>(buf);
		for (int i = 0; i < 3; i++) {
			if (players[i].id == packet->id) {
				players[i].score += 1;
				currentJewelyNum++;
				if (!currentStage.jewely.empty()) {
					currentStage.currentVisibleJewely = currentStage.jewely.front();
					currentStage.jewely.pop();
				}
				break;
			}
		}
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
	case C2SChangRole:
		size = sizeof(C2SRolePacket);
		packet = new char[size];
		memcpy(packet, buf, size);
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

	case C2SEndout:
		size = sizeof(C2SEndPacket);
		packet = new char[size];
		memcpy(packet, buf, size);
		
		break;
	default:
		// Packet Error
		break;
	}

	if (packet != nullptr) {
		send(c_socket, packet, size, NULL);
		delete[] packet;
		if (reinterpret_cast<char*>(buf)[0] == C2SEndout)
		{
			exit(1);
		}
	}
}

void ConstructPacket(void* buf, int ioSize)
{
	int restSize = ioSize + prevSize;
	int needSize = 0;
	while (restSize != 0) {
		needSize = GetPacketSize(reinterpret_cast<char*>(buf)[0]);
		if (needSize < 0)
			return;
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
	MessageBox(hWnd, (LPWSTR)lpMsgBuf, _T("서버주소 오류!"), NULL);
	LocalFree(lpMsgBuf);
}

int GetPacketSize(char packetType)
{
	int retVal = -1;
	switch (packetType)
	{
	case S2CLoading:
	case S2CAddPlayer:
	case S2CIntoDoor:
	case S2CEatJewely:
		retVal = sizeof(S2CPlayerPacket);
		break;
	case S2CChangeRole:
	case S2CSelectRole:
		retVal = sizeof(S2CRolePacket);
		break;
	case S2CChangeStage:
		retVal = sizeof(S2CChangeStagePacket);
		break;
	case S2CStageTimePass:
		retVal = sizeof(S2CStageTimePassPacket);
		break;
	case S2CStageTimeout:
		retVal = sizeof(typePacket);
		break;
	case S2CMove_IDLE:
	case S2CMove_JUMP:
	case S2CMove_LEFT:
	case S2CMove_RIGHT:
		retVal = sizeof(MovePacket);
		break;
	case S2CExitGame:
	case S2CEndout:
	case S2CDoorVisible:
		retVal = sizeof(typePacket);
		break;
		/*case S2CJewelryVisibility:
			retVal = sizeof(S2CJewelryVisibilityPacket);
			break;*/

	case C2SEndout:
		retVal = sizeof(S2CEndPacket);
		break;
	default:
		break;
	}
	return retVal;
}