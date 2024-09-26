#pragma once

#include "../FWE_Server/protocol.h"
#include "stdafx.h"
#include "ImageMgr.h"
#include "StageMgr.h"

HINSTANCE g_hInst;
ImageMgr myImageMgr;
StageMgr myStageMgr;
Stage currentStage;

WSADATA WSAData;
SOCKET c_socket;

int stageIndex = 0;
int currneClientNum = 1;


int prevSize = 0;
int myId = -1;
bool doorVisible = false;
char recvBuf[MAX_BUF_SIZE];
static BOOL isArrow = true;
int currentJewelyNum = 0;
bool myCharacterOn = true;

HANDLE selectMyCharacter = NULL;
HANDLE changeStageEvent = NULL;
HANDLE idleStateEvent = NULL;
HANDLE jumpEvent = NULL;

HWND g_hWnd;
DWORD WINAPI ClientrecvThread(LPVOID arg);


// 프로그램 최초 실행시 변수 초기화 및 윈도우 생성
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	LPCTSTR IpszClass = L"fire boy and water girl";
	LPCTSTR IpszWindowName = L"window programming";

	g_hInst = hInstance;

	// 윈도우 값 초기화
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbWndExtra = 0;
	WndClass.cbClsExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = IpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	// 전체 이미지 초기화
	myImageMgr.LoadImages();

	// 메인 윈도우 생성
	hWnd = CreateWindow(IpszClass, IpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WID, WINDOW_HEI, NULL, (HMENU)NULL, hInstance, NULL);

	// 메인 윈도우 Set Visible
	ShowWindow(hWnd, nCmdShow);

	// 메인 윈도우 Update Data
	UpdateWindow(hWnd);

	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
		return 1;
	c_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	HANDLE chandle;
	chandle = CreateThread(NULL, 0, ClientrecvThread, NULL, 0, NULL);

	selectMyCharacter = CreateEvent(NULL, TRUE, FALSE, NULL);
	changeStageEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	idleStateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	jumpEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(selectMyCharacter);
	ResetEvent(changeStageEvent);
	ResetEvent(idleStateEvent);
	ResetEvent(jumpEvent);

	// 스테이지 열기
	currentStage = myStageMgr.getStage(stageIndex);

	// 윈도우 Main Message Loop
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return static_cast<int>(Message.wParam);
}

HWND start_button, retry_button, end_button, next_button, server_addr;
HWND selectRoleLeftArrow;
HWND selectBtn;
HWND btnend;
HWND selectRoleRightArrow;
BOOL back = FALSE;


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static HDC hDC;
	static HDC memdc, backMemDC, memDC;
	static HBITMAP hBitmap, oldBitmap;

	g_hWnd = hWnd;

	switch (uMsg) {
	case WM_CREATE: {	// 프로그램 최초 실행에서 1회 실행
		start_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 450, 600, 158, 60, hWnd, (HMENU)BTN_START, g_hInst, NULL);

		SendMessage(start_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.buttonimg));


		static HFONT s_hFont = (HFONT)NULL;
		NONCLIENTMETRICS nonClientMetrics;
		ZeroMemory(&nonClientMetrics, sizeof(nonClientMetrics));
		nonClientMetrics.cbSize = sizeof(NONCLIENTMETRICS);
		s_hFont = CreateFontIndirect(&nonClientMetrics.lfCaptionFont);

		server_addr = CreateWindow(L"edit", L"127.0.0.1", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 450, 550, 158, 30, hWnd, (HMENU)EDIT_SERVER_ADDR, g_hInst, NULL);
		SendMessage(server_addr, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));

		selectRoleRightArrow = CreateWindow(L"button", L"right", WS_CHILD | BS_PUSHBUTTON | BS_BITMAP, 330, 330, 80, 41, hWnd, (HMENU)BTN_LEFT_ARROW, g_hInst, NULL);
		selectRoleLeftArrow = CreateWindow(L"button", L"Left", WS_CHILD | BS_PUSHBUTTON | BS_BITMAP, 50, 330, 80, 41, hWnd, (HMENU)BTN_RIGHT_ARROW, g_hInst, NULL);
		selectBtn = CreateWindow(L"button", L"RoleSelect", WS_CHILD | BS_PUSHBUTTON | BS_BITMAP, 550, 620, 120, 45, hWnd, (HMENU)BTN_SELECT, g_hInst, NULL);
		btnend = CreateWindow(L"button", L"end", WS_CHILD | BS_PUSHBUTTON | BS_BITMAP, 10, 10, 242, 142, hWnd, (HMENU)BTN_STOP, g_hInst, NULL);
		SendMessage(selectRoleLeftArrow, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.leftArrow));
		SendMessage(selectRoleRightArrow, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.rightArrow));
		SendMessage(selectBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.selectBtn));
		SendMessage(btnend, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.btnend));


		SetTimer(hWnd, 1, 30, NULL);
		SetTimer(hWnd, 2, 100, NULL);
		SetTimer(hWnd, 3, 1000, NULL);
		SetTimer(hWnd, 5, 1, NULL);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case BTN_START:
			wchar_t wc_s_addr[256];
			char c_s_addr[256];
			size_t t;
			GetWindowText(server_addr, wc_s_addr, 256);
			wcstombs_s(&t, c_s_addr, wc_s_addr, 256);

			if (NetworkInit(hWnd, c_s_addr)) {
				DestroyWindow(start_button);
				DestroyWindow(server_addr);

				if (stageIndex < STAGE_ROLE) {
					stageIndex = STAGE_LOADING;
					SetEvent(changeStageEvent);
				}
			}
			else {
				SetWindowText(server_addr, LPCWSTR());
			}
			break;
		case BTN_RESTART:
			//stageTime = 300;
			for (PLAYER& pl : players)
				pl.on = true;
			SetTimer(hWnd, 3, 1000, NULL);
			back = FALSE;
			currentStage.time_over = FALSE;
			myStageMgr.ResetStage();
			DestroyWindow(retry_button);
			DestroyWindow(end_button);
			break;

		case BTN_LEFT_ARROW:
		{
			C2SRolePacket makePacket;
			makePacket.type = static_cast<int>(PACKET_TYPE_C2S::ChangRole);
			makePacket.size = sizeof(C2SRolePacket);
			//f w e
			if (players[0].role == 'e') {
				players[0].role = 'w';
			}
			else if (players[0].role == 'w') {
				players[0].role = 'f';
			}
			else if (players[0].role == 'f') {
				players[0].role = 'e';
			}
			makePacket.role = static_cast<char>(players[0].role);
			makePacket.id = static_cast<char>(myId);
			send(c_socket, reinterpret_cast<char*>(&makePacket), sizeof(makePacket), NULL);
			// SendPacket(&makePacket);
		}
		break;
		case BTN_RIGHT_ARROW:
		{
			//fwe
			C2SRolePacket makePacket;
			makePacket.type = static_cast<int>(PACKET_TYPE_C2S::ChangRole);
			makePacket.size = sizeof(C2SRolePacket);
			if (players[0].role == 'e') {
				players[0].role = 'f';
			}
			else if (players[0].role == 'w') {
				players[0].role = 'e';
			}
			else if (players[0].role == 'f') {
				players[0].role = 'w';
			}
			makePacket.role = static_cast<char>(players[0].role);
			makePacket.id = static_cast<char>(myId);
			send(c_socket, reinterpret_cast<char*>(&makePacket), sizeof(makePacket), NULL);
			// SendPacket(&makePacket);
		}
		break;
		case BTN_SELECT:
		{
			C2SRolePacket makePacket;
			makePacket.size = sizeof(C2SRolePacket);
			makePacket.type = static_cast<int>(PACKET_TYPE_C2S::SelectRole);
			makePacket.role = static_cast<char>(players[0].role);
			makePacket.id = static_cast<char>(myId);
			send(c_socket, reinterpret_cast<char*>(&makePacket), sizeof(makePacket), NULL);
			// SendPacket(&makePacket);
		}
		break;
		case BTN_QUIT:
			back = FALSE;
			currentStage.time_over = FALSE;
			DestroyWindow(retry_button);
			DestroyWindow(end_button);
			PostQuitMessage(0);
			break;
		case BTN_STOP:
			PostQuitMessage(0);
			typePacket endPacket;
			endPacket.type = static_cast<int>(PACKET_TYPE_C2S::Endout);
			SendPacket(&endPacket);
			break;
		}

	case WM_TIMER:
		switch (wParam) {
		case 1:
		{
			if (currentStage.currentVisibleJewely.GetVisible() && currentStage.currentVisibleJewely.ChangeFrame(1, false))
				currentStage.currentVisibleJewely.image_x = 0;
			
			for (OBJECT& t : currentStage.Trap)
				if (t.GetVisible())
					t.ChangeFrame(1, true);

			if (currentStage.Die.GetVisible() && STAGE_01 <= currentStage.stage)
			{
				if (currentStage.Die.ChangeFrame(1, false))
				{
					back = TRUE;
					currentStage.Die.SetVisible(FALSE);
					currentStage.Die.image_x = 0;

					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					KillTimer(hWnd, 3);

					retry_button = CreateWindow(L"button", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 300, 500, 200, 100, hWnd, (HMENU)BTN_RESTART, g_hInst, NULL);
					SendMessage(retry_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.retryimg));
					end_button = CreateWindow(L"button", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 700, 500, 200, 100, hWnd, (HMENU)BTN_QUIT, g_hInst, NULL);
					SendMessage(end_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.endimg));
					mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
				}
			}
		}
		break;
		case 2:				// 캐릭터 프레임
			for (PLAYER& pl : players) pl.Frame = (pl.Frame + 1) % 8;
			break;
		case 3:
			if (myStageMgr.IsTimeoutStageEnd == true)
			{
				currentStage.time_over = TRUE;
				back = TRUE;
				retry_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 300, 500, 200, 100, hWnd, (HMENU)BTN_RESTART, g_hInst, NULL);
				SendMessage(retry_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.retryimg));
				end_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 700, 500, 200, 100, hWnd, (HMENU)BTN_QUIT, g_hInst, NULL);
				SendMessage(end_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.endimg));
				mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
			}
			break;
		case 4:
		{
			DWORD retVal = WaitForSingleObject(selectMyCharacter, 0);
			if (retVal == WAIT_OBJECT_0) {

				char buf[10] = { static_cast<char>(myId) + '0' };
				SetWindowTextA(hWnd, buf);

				ResetEvent(selectMyCharacter);
				ShowWindow(selectRoleLeftArrow, SW_HIDE);
				ShowWindow(selectRoleRightArrow, SW_HIDE);
				ShowWindow(selectBtn, SW_HIDE);
				KillTimer(hWnd, 4);
			}
		}
		break;
		case 5:
		{
			DWORD retVal = WaitForSingleObject(changeStageEvent, 0);
			if (retVal == WAIT_OBJECT_0) {
				currentStage = myStageMgr.getStage(stageIndex);
				ResetEvent(changeStageEvent);
			}
		}
		break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		keybuffer[wParam] = TRUE;
		Move();
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hDC);
		break;

	case WM_KEYUP:
		hDC = GetDC(hWnd);
		keybuffer[wParam] = FALSE;
		Move();
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hDC);
		break;

	case WM_PAINT: {
		hDC = BeginPaint(hWnd, &ps);
		backMemDC = CreateCompatibleDC(hDC);
		memDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, WINDOW_WID, WINDOW_HEI);
		oldBitmap = (HBITMAP)SelectObject(backMemDC, hBitmap);
		PatBlt(backMemDC, 0, 0, WINDOW_WID, WINDOW_HEI, WHITENESS);

		myImageMgr.DrawMap(&backMemDC, stageIndex, currentStage);

		if (STAGE_ROLE == currentStage.stage) {
			if (isArrow) {
				ShowWindow(selectRoleLeftArrow, SW_SHOW);
				ShowWindow(selectRoleRightArrow, SW_SHOW);
				ShowWindow(selectBtn, SW_SHOW);
				SetTimer(hWnd, 4, 10, NULL);
				isArrow = false;
			}
		}
		else if (STAGE_ROLE < currentStage.stage && currentStage.stage < RESULT) {
			myImageMgr.DrawPlayers(&backMemDC, currentStage);
			myImageMgr.DrawTimer(&backMemDC, StageMgr::EndStageTime - StageMgr::StageTimepass);
			myImageMgr.DrawScore(&backMemDC);
			if (currentStage.clear)	back = TRUE;
			if (currentStage.time_over) myImageMgr.timeout.Draw(backMemDC, 400, 200, 400, 250, 0, 0, 486, 286);
			if (currentStage.stair)
			{
				myImageMgr.red_stair.Draw(backMemDC, currentStage.door.x, currentStage.door.y + 30, 50, 80, currentStage.door.image_x, 0, 50, 73);
				for (PLAYER& pl : players)
					pl.on = false;
			}
			if (back)
			{
				myImageMgr.backimg.Draw(backMemDC, 0, 0, WINDOW_WID, WINDOW_HEI, 0, 0, WINDOW_WID, WINDOW_HEI);
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				currentStage.clear = FALSE;
				mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
			}
		}
		else if (currentStage.stage == RESULT) {
			ShowWindow(btnend, SW_SHOW);
		}


		BitBlt(backMemDC, 0, 0, WINDOW_WID, WINDOW_HEI, memDC, 0, 0, SRCCOPY);
		BitBlt(hDC, 0, 0, WINDOW_WID, WINDOW_HEI, backMemDC, 0, 0, SRCCOPY);
		DeleteObject(SelectObject(backMemDC, oldBitmap));
		DeleteObject(hBitmap);
		DeleteDC(backMemDC);
		DeleteDC(memDC);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

DWORD WINAPI ClientrecvThread(LPVOID arg)
{
	while (true) {
		int recvRetVal = recv(c_socket, recvBuf + prevSize, MAX_BUF_SIZE - prevSize, 0);
		if (recvRetVal != 0 && recvRetVal != -1) {
			ConstructPacket(recvBuf, recvRetVal);
		}
		else {
			WSAGetLastError();
		}
	}
	return 0;
}