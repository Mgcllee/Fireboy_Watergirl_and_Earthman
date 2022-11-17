#pragma once
#include"stdafx.h"
#include "ImageMgr.h"
#include "StageMgr.h"
#include"protocol.h"
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

char recvBuf[MAX_BUF_SIZE] = { 0 };

HWND g_hWnd;
DWORD WINAPI ClientrecvThread (LPVOID arg);


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
	hWnd = CreateWindow(IpszClass, IpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1200, 800, NULL, (HMENU)NULL, hInstance, NULL);

	// 메인 윈도우 Set Visible
	ShowWindow(hWnd, nCmdShow);

	// 메인 윈도우 Update Data
	UpdateWindow(hWnd);

	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
		return 1;
	c_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	HANDLE chandle;
	chandle = CreateThread(NULL, 0, ClientrecvThread, reinterpret_cast<LPVOID>(c_socket), 0,NULL);
	

	// 스테이지 열기
	currentStage = myStageMgr.getStage(stageIndex);

	// 윈도우 Main Message Loop
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

/*
	[메인 윈도우 작업 함수]
	1. WM_CREATE : 최초 초기화

	2. WM_COMMAND : 키 입력
					(IDC_BUTTON1 : Title 화면 시작 버튼,
					 IDC_BUTTON2 : Game Over 화면에서 재시작 버튼)
					 IDC_BUTTON3 : Game Over 화면에서 프로그램 종료 버튼,
					 IDC_BUTTON4 : Stage Clear 성공시 다음 스테이지로 이동)

	3. WM_TIMER : 타이머 호출시 실행, 내부 switch문에서 timer의 ID를 사용해 해당 작업을 수행
					(Timer ID : case WM_CREATE 에 있는 SetTimer 함수의 2번째 파라미터)
					(Timer ID 1 : User 입력에 따른 캐릭터 이동과 충돌체크,
					 Timer ID 2 : 캐릭터(Fire boy, Water girl)의 애니메이션 프레임 (총 8개)
					 Timer ID 3 : 오브젝트 애니메이션 전용
					 Timer ID 4 : 캐릭터 출구, Stage_03의 이동 발판 애니메이션 (Timer ID 3 와 호출 시간이 다름)
					 Timer ID 5 : 제한 시간 (time변수) 초과할 경우 화면 갱신 ()

	4. WM_KEYDOWN : 키가 눌렸을 때 동작

	5. WM_KEYUP : 키가 (눌렸다가) 올라갈 때 동작

	6. WM_PAINT : 메인 윈도우에 오브젝트 그리기

	6. WM_DESTROY : 메인 윈도우 종료 (== 프로그램 종료)
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static HDC memdc;
	static HBITMAP hbitmap;

	g_hWnd = hWnd;

	static HWND start_button, retry_button, end_button, next_button, server_addr;
	static HWND selectRoleLeftArrow;
	static HWND selectRoleRightArrow;
	static HWND selectBtn;
	static BOOL back = FALSE;
	static BOOL isArrow = true;

	static int time = 300;

	
	currentStage = myStageMgr.getStage(stageIndex);

	switch (uMsg) {
	case WM_CREATE: {	// 프로그램 최초 실행에서 1회 실행
		start_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 450, 600, 158, 60, hWnd, (HMENU)BTN_START, g_hInst, NULL);
		SendMessage(start_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.buttonimg));

		static HFONT s_hFont = (HFONT)NULL;
		NONCLIENTMETRICS nonClientMetrics;
		ZeroMemory(&nonClientMetrics, sizeof(nonClientMetrics));
		nonClientMetrics.cbSize = sizeof(NONCLIENTMETRICS);
		s_hFont = CreateFontIndirect(&nonClientMetrics.lfCaptionFont);

		server_addr = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 450, 550, 158, 30, hWnd, (HMENU)EDIT_SERVER_ADDR, g_hInst, NULL);
		SendMessage(server_addr, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));

		SetTimer(hWnd, 1, 30, NULL);
		SetTimer(hWnd, 2, 100, NULL);
		SetTimer(hWnd, 3, 50, NULL);
		SetTimer(hWnd, 4, 50, NULL);
		SetTimer(hWnd, 5, 1000, NULL);
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
				if(stageIndex < STAGE_ROLE)
					stageIndex = STAGE_LOADING;
				currentStage = myStageMgr.getStage(stageIndex);
			}
			else {
				SetWindowText(server_addr, LPCWSTR());
			}
			break;
		case BTN_RESTART:
			time = 300;
			for (PLAYER& pl : players)
				pl.on = true;


			SetTimer(hWnd, 1, 30, NULL);
			SetTimer(hWnd, 2, 100, NULL);
			SetTimer(hWnd, 3, 50, NULL);
			SetTimer(hWnd, 4, 50, NULL);
			SetTimer(hWnd, 5, 1000, NULL);
			back = FALSE;
			currentStage.time_over = FALSE;
			DestroyWindow(retry_button);
			DestroyWindow(end_button);
			break;

		case BTN_LEFT_ARROW:
		{	
			C2SRolePacket makePacket;
			makePacket.type = C2SChangeRole;
			makePacket.role = 'f'; // 아직 로직 안 짬
			players[0].role = 'e';
			SendPacket(&makePacket);
		}
		break;
		case BTN_RIGHT_ARROW:
		{
			C2SRolePacket makePacket;
			makePacket.type = C2SChangeRole;
			makePacket.role = 'f'; 
			players[0].role = 'w';// 아직 로직 안 짬
			SendPacket(&makePacket);
		}
		break;
		case BTN_SELECT:
		{
			C2SRolePacket makePacket;
			makePacket.type = C2SSelectRole;
			makePacket.role = 'f'; // 아직 로직 안 짬 수정해야됨
			SendPacket(&makePacket);

			
			currentStage = myStageMgr.getStage(stageIndex = STAGE_01);
		}
		break;
		case BTN_QUIT:
			back = FALSE;
			currentStage.time_over = FALSE;
			DestroyWindow(retry_button);
			DestroyWindow(end_button);
			PostQuitMessage(0);
			DeleteObject(hbitmap);
			DeleteDC(memdc);
			break;

		case BTN_NEXT_STAGE:
			back = FALSE;
			currentStage.clear = FALSE;
			for (PLAYER& pl : players)
				pl.on = true;
			time = 300;
			currentStage = myStageMgr.getStage(++stageIndex);

			currentStage.red_total = currentStage.Red_Jewel.size();
			currentStage.blue_total = currentStage.Blue_Jewel.size();
			currentStage.count = 0;
			SetTimer(hWnd, 1, 30, NULL);
			SetTimer(hWnd, 2, 100, NULL);
			SetTimer(hWnd, 3, 50, NULL);
			SetTimer(hWnd, 4, 50, NULL);
			SetTimer(hWnd, 5, 1000, NULL);
			DestroyWindow(next_button);
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			// 캐릭터 이동과 충돌체크
			for (PLAYER& pl : players) {
				for (auto& bj : currentStage.Blue_Jewel) {
					if (bj.Collision(pl)) {
						bj.SetVisible(false);
					}
				}
			}

			if (STAGE_01 <= currentStage.stage)
			{
				for (PLAYER& pl : players) {
					for (auto& t : currentStage.Trap) {
						if (t.Collision(pl)) {
							pl.on = FALSE;
							currentStage.Die.SetVisible(true);
							currentStage.Die.x = t.x;
							currentStage.Die.y = t.y - 100;
						}
					}
				}
			}
			break;
		case 2:				// 캐릭터 프레임
			for (PLAYER& pl : players) pl.Frame = (pl.Frame + 1) % 9;
			break;
		case 3:				// 보석, 함정, 사망, 문 애니메이션 처리와 스테이지 클리어 조건 충족시 후처리(Next 윈도우와 배경처리)
			for (OBJECT& bj : currentStage.Blue_Jewel)
				if (bj.GetVisible() && bj.ChangeFrame(1, true))
					bj.image_x = 0;

			for (OBJECT& rj : currentStage.Red_Jewel)
				if (rj.GetVisible() && rj.ChangeFrame(1, true))
					rj.image_x = 0;

			for (OBJECT& t : currentStage.Trap)
				if (t.GetVisible())
					t.ChangeFrame(1, true);

			// Fire bot, Water girl 중 1명이라도 사망 && 현재 Stage가 1 이상인 경우 (Stage 0은 Title 화면)
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
					KillTimer(hWnd, 4);
					KillTimer(hWnd, 5);

					retry_button = CreateWindow(L"button", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 300, 500, 200, 100, hWnd, (HMENU)BTN_RESTART, g_hInst, NULL);
					SendMessage(retry_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.retryimg));
					end_button = CreateWindow(L"button", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 700, 500, 200, 100, hWnd, (HMENU)BTN_QUIT, g_hInst, NULL);
					SendMessage(end_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.endimg));
					mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
				}
			}

			// 스테이스 클리어 후 문으로 들어가는 애니메이션
			// if (currentStage.red_door.Collision(players[1]) && currentStage.blue_door.Collision(players[0]))
			//{
			//	// if (currentStage.red_door.ChangeFrame(1) && currentStage.blue_door.ChangeFrame(1)) {
			//	if (currentStage.blue_door.ChangeFrame(1)) {
			//		currentStage.stair = FALSE;
			//		currentStage.red_door.image_x = 0;
			//		currentStage.blue_door.image_x = 0;
			//		next_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 500, 400, 200, 100, hWnd, (HMENU)BTN_NEXT_STAGE, g_hInst, NULL);
			//		SendMessage(next_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.clear_img));
			//	}
			//}

			for (PLAYER& pl : players) {
				for (OBJECT& btn : currentStage.button)
					if (btn.GetVisible() && btn.Collision(pl))
						btn.ChangeFrame(1, false);
			}
			break;
		case 4:
			/*if (currentStage.red_door.GetVisible())
				currentStage.red_door.ChangeFrame(1);
			else
				currentStage.red_door.ChangeFrame(-1);*/

			if (currentStage.blue_door.Collision(players[1]))
				currentStage.blue_door.ChangeFrame(1, false);
			else if (currentStage.blue_door.image_x != 0) {

				if (currentStage.blue_door.image_x + currentStage.blue_door.imageMoveWid == currentStage.blue_door.MaxWid)
					currentStage.blue_door.image_x -= currentStage.blue_door.imageMoveWid;

				currentStage.blue_door.ChangeFrame(-1, false);
			}

			// Stage_03() 에서 나오는 X축 이동 바 (block1.PNG 파일 참고)
			for (OBJECT& block : currentStage.block)
				if (block.GetVisible())
					block.ChangeFrame(-1, false);
				else
					block.ChangeFrame(1, false);
			break;

		case 5:
			//타임아웃시 할 동작

			if (--time == 0)
			{
				currentStage.time_over = TRUE;
				back = TRUE;
				retry_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 300, 500, 200, 100, hWnd, (HMENU)BTN_RESTART, g_hInst, NULL);
				SendMessage(retry_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.retryimg));
				end_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 700, 500, 200, 100, hWnd, (HMENU)BTN_QUIT, g_hInst, NULL);
				SendMessage(end_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.endimg));
				mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
				KillTimer(hWnd, 5);
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
		hbitmap = CreateCompatibleBitmap(hDC, 1200, 800);
		memdc = CreateCompatibleDC(hDC);
		SelectObject(memdc, hbitmap);

		myImageMgr.DrawMap(&memdc, currentStage.stage, currentStage);

		if (STAGE_ROLE == currentStage.stage) {
			if (isArrow) {
				selectRoleLeftArrow = CreateWindow(L"button", L"RoleSelect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 50, 280, 80, 41, hWnd, (HMENU)BTN_LEFT_ARROW, g_hInst, NULL);
				selectRoleRightArrow = CreateWindow(L"button", L"RoleSelect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 330, 280, 80, 41, hWnd, (HMENU)BTN_RIGHT_ARROW, g_hInst, NULL);
				selectBtn = CreateWindow(L"button", L"RoleSelect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 500, 620, 120, 45, hWnd, (HMENU)BTN_SELECT, g_hInst, NULL);
				SendMessage(selectRoleLeftArrow, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.leftArrow));
				SendMessage(selectRoleRightArrow, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.rightArrow));
				SendMessage(selectBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.selectBtn));
				isArrow = false;
			}
		}
		else if (STAGE_ROLE < currentStage.stage) {
			if (selectRoleLeftArrow != nullptr) {
				DestroyWindow(selectRoleLeftArrow);
				DestroyWindow(selectRoleRightArrow);
				DestroyWindow(selectBtn);
			}

			myImageMgr.DrawPlayers(&memdc, currentStage);
			myImageMgr.DrawTimer(&memdc, time);

			for (auto& rj : currentStage.Red_Jewel) {
				if (rj.GetVisible())
					myImageMgr.Jewelry_red.Draw(memdc, rj.x, rj.y, rj.wid, rj.hei, rj.image_x, 0, 28, 24);
			}
			for (auto& bj : currentStage.Blue_Jewel) {
				if (bj.GetVisible()) {
					myImageMgr.Jewelry_blue.Draw(memdc, bj.x, bj.y, bj.wid, bj.hei, bj.image_x, 0, 28, 24);
				}
			}

			// 사망시 연기 Anim
			if (currentStage.Die.GetVisible())	myImageMgr.die.Draw(memdc, currentStage.Die.x, currentStage.Die.y, 100, 100, currentStage.Die.image_x, currentStage.Die.image_y, 159, 89);
			if (currentStage.clear)	back = TRUE;
			if (currentStage.time_over) myImageMgr.timeout.Draw(memdc, 400, 200, 400, 250, 0, 0, 486, 286);
			if (currentStage.stair)
			{
				myImageMgr.red_stair.Draw(memdc, currentStage.red_door.x, currentStage.red_door.y + 30, 50, 80, currentStage.red_door.image_x, 0, 50, 73);
				myImageMgr.blue_stair.Draw(memdc, currentStage.blue_door.x, currentStage.red_door.y + 30, 50, 80, currentStage.red_door.image_x, 0, 54, 77);
				for (PLAYER& pl : players)
					pl.on = false;
			}
			if (back)
			{
				myImageMgr.backimg.Draw(memdc, 0, 0, 1200, 800, 0, 0, 1200, 800);
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				currentStage.clear = FALSE;
				mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
			}
		}

		//if (currentStage.clear == FALSE && back == FALSE)
		//{
		//	for (int i = 0; i < 90; i++) // 0~19까지 파랑 가운데 물 20~29까지 파랑 왼.오 |||| 30~49까지 빨강 가운데 물 50~59까지 빨강물 왼.오 60~79 초록 가운데 80~89 초록왼.오
		//	{
		//		if (currentStage.Trap[i].GetVisible())
		//		{
		//			
		//			if (i < 20)
		//			{
		//				myImageMgr.blue_water_center.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 23, 14);
		//			}
		//			if (i >= 20 && i < 25)
		//			{
		//				myImageMgr.blue_water_left.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 23, 23);
		//			}
		//			if (i >= 25 && i < 30)
		//			{
		//				myImageMgr.blue_water_right.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 23, 23);
		//			}
		//			
		//			if (i >= 30 && i < 50)
		//			{
		//				myImageMgr.red_water_center.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 21, 14);
		//			}
		//			if (i >= 50 && i < 55)
		//			{
		//				myImageMgr.red_water_left.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 24, 25);
		//			}
		//			if (i >= 55 && i < 60)
		//			{
		//				myImageMgr.red_water_right.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 24, 25);
		//			}
		//			
		//			if (i >= 60 && i < 80)
		//			{
		//				myImageMgr.green_water_center.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 23, 14);
		//			}
		//			if (i >= 80 && i < 85)
		//			{
		//				myImageMgr.green_water_left.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 24, 25);
		//			}
		//			if (i >= 85 && i < 90)
		//			{
		//				myImageMgr.green_water_right.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 24, 25);
		//			}
		//		}
		//	}
		//}

		BitBlt(hDC, 0, 0, 1200, 800, memdc, 0, 0, SRCCOPY);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
		DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
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