#pragma once
#include"stdafx.h"
#include "ImageMgr.h"
#include "StageMgr.h"

HINSTANCE g_hInst;
ImageMgr myImageMgr;
StageMgr myStageMgr;
Stage currentStage;

int stageIndex = 0;

int currneClientNum = 1;

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
bool isArrow = true;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static HDC memdc;
	static HBITMAP hbitmap;

	static HWND start_button, retry_button, end_button, next_button;
	static HWND selectRoleLeftArrow, selectRoleRightArrow;
	static HWND selectBtn;
	static BOOL back = FALSE;
	static int time = 300;
	int blue_count = 0;
	int red_count = 0;
	static int stair_red_x = 0, stair_blue_x = 0;

	// 현재 스테이지 획득
	currentStage = myStageMgr.getStage(stageIndex);

	switch (uMsg) {
	case WM_CREATE: {	// 프로그램 최초 실행에서 1회 실행
		start_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 450, 600, 158, 60, hWnd, (HMENU)BTN_START, g_hInst, NULL);
		SendMessage(start_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.buttonimg));

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
			stageIndex = 1;
			SetTimer(hWnd, 5, 1000, NULL);
			DestroyWindow(start_button);
			break;
		case BTN_RESTART:
			time = 300;
			fire.on = TRUE;
			water.on = TRUE;
		case BTN_LEFT_ARROW:
			//캐릭터 변경 돼야함
		{
			int a = 10;
		}
		break;
		case BTN_RIGHT_ARROW:
			//캐릭터 변경 돼야함
			break;

			currentStage = myStageMgr.getStage(stageIndex);

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
			fire.on = TRUE;
			water.on = TRUE;
			time = 300;
			currentStage = myStageMgr.getStage(++stageIndex);

			/*for (int i = 0; i < 20; i++)
			{
				if (Jwewlry.find(i)->second.On)
				{
					if (i < 10)
					{
						red_count++;
					}
					else if (i >= 10)
					{
						blue_count++;
					}
				}
			}*/

			currentStage.red_total = red_count;
			currentStage.blue_total = blue_count;
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
		case 1:				// 캐릭터 이동과 충돌체크
			currentStage.Wid_Move();
			currentStage.Jump();
			currentStage.Foot();
			currentStage.Push();

			for (auto& rj : currentStage.Red_Jewel) {
				if (rj.Collision(fire)) {
					rj.SetVisible(false);
				}
			}

			for (auto& bj : currentStage.Blue_Jewel) {
				if (bj.Collision(water)) {
					bj.SetVisible(false);
				}
			}

			if (currentStage.stage > 0)
			{
				// 0~19까지 파랑 가운데 20~24 파랑왼 25~29파랑오
				// 30~49 빨강 가운데 50~54~빨강왼 55~59빨강오 
				// 60~79초록 가운데 80~84초록왼 85~89초록오

				for (auto& t : currentStage.Trap) {
					if (t.Collision(fire)) {
						fire.on = FALSE;
						currentStage.Die.SetVisible(true);
						currentStage.Die.x = t.x;
						currentStage.Die.y = t.y - 100;
					}
				}

				/*	for (int i = 0; i < 90; i++)
					{
						if (i < 30)
						{
							if (((fire.x - 50 <= Trap[i].x + 28 && fire.x - 50 >= Trap[i].x) || (fire.x <= Trap[i].x + 28 && fire.x >= Trap[i].x)) && ((fire.y - 80 >= Trap[i].y && fire.y - 80 <= Trap[i].y + 25) || (fire.y >= Trap[i].y && fire.y <= Trap[i].y + 25)))
							{
								fire.on = FALSE;
								Die.On = TRUE;
								Die.x = Trap[i].x;
								Die.y = Trap[i].y-100;
							}
						}

						if (i >= 30 && i < 60)
						{
							if (((water.x - 50 <= Trap[i].x + 25 && water.x - 50 >= Trap[i].x) || (water.x <= Trap[i].x + 25 && water.x >= Trap[i].x)) && ((water.y - 80 >= Trap[i].y && water.y - 80 <= Trap[i].y + 25) || (water.y >= Trap[i].y && water.y <= Trap[i].y + 25)))
							{
								water.on = FALSE;
								Die.On = TRUE;
								Die.x = Trap[i].x;
								Die.y = Trap[i].y-100;
							}
						}

						if (i >= 60 && i < 90)
						{
							if (((fire.x - 50 <= Trap[i].x + 25 && fire.x - 50 >= Trap[i].x) || (fire.x <= Trap[i].x + 25 && fire.x >= Trap[i].x)) && ((fire.y - 80 >= Trap[i].y && fire.y - 80 <= Trap[i].y + 25) || (fire.y >= Trap[i].y && fire.y <= Trap[i].y + 25)))
							{
								fire.on = FALSE;
								Die.On = TRUE;
								Die.x = Trap[i].x;
								Die.y = Trap[i].y-100;
							}
							if (((water.x - 50 <= Trap[i].x + 25 && water.x - 50 >= Trap[i].x) || (water.x <= Trap[i].x + 25 && water.x >= Trap[i].x)) && ((water.y - 80 >= Trap[i].y && water.y - 80 <= Trap[i].y + 25) || (water.y >= Trap[i].y && water.y <= Trap[i].y + 25)))
							{
								water.on = FALSE;
								Die.On = TRUE;
								Die.x = Trap[i].x;
								Die.y = Trap[i].y-100;
							}
						}
					}*/

					/*for (int i = 0; i < 5; i++)
					{
						if (((fire.x - 50 <= button[i].x + 40 && fire.x - 50 >= button[i].x) || (fire.x <= button[i].x + 40 && fire.x >= button[i].x)) && ((fire.y - 80 >= button[i].y - button[i].image_y && fire.y - 80 <= button[i].y) || (fire.y >= button[i].y - button[i].image_y && fire.y <= button[i].y)))
						{
							button[i].On = TRUE;
						}
						else if (((water.x - 50 <= button[i].x + 40 && water.x - 50 >= button[i].x) || (water.x <= button[i].x + 40 && water.x >= button[i].x)) && ((water.y - 80 >= button[i].y - button[i].image_y && water.y - 80 <= button[i].y) || (water.y >= button[i].y - button[i].image_y && water.y <= button[i].y)))
						{
							button[i].On = TRUE;
						}
						else if (fire.x >= button[i].x && fire.x - 50 <= button[i].x && ((fire.y - 80 >= button[i].y && fire.y - 80 <= button[i].y - -button[i].image_y) || (fire.y >= button[i].y - button[i].image_y && fire.y <= button[i].y - -button[i].image_y)))
						{
							button[i].On = TRUE;
						}
						else if (water.x >= button[i].x && water.x - 50 <= button[i].x && ((water.y - 80 >= button[i].y && water.y - 80 <= button[i].y - -button[i].image_y) || (water.y >= button[i].y - button[i].image_y && water.y <= button[i].y - -button[i].image_y)))
						{
							button[i].On = TRUE;
						}
						else
						{
							button[i].On = FALSE;
						}
					}
					for (int i = 0; i < 5; i++)
					{
						if (((Rt.x - 50 <= button[i].x + 40 && Rt.x - 50 >= button[i].x) || (Rt.x <= button[i].x + 40 && Rt.x >= button[i].x)) && ((Rt.y - 50 >= button[i].y - button[i].image_y && Rt.y - 50 <= button[i].y) || (Rt.y >= button[i].y - button[i].image_y && Rt.y <= button[i].y)))
						{
							button[i].On = TRUE;
						}
						else if (Rt.x >= button[i].x && Rt.x - 50 <= button[i].x && ((Rt.y - 80 >= button[i].y && Rt.y - 80 <= button[i].y - -button[i].image_y) || (Rt.y >= button[i].y - button[i].image_y && Rt.y <= button[i].y - -button[i].image_y)))
						{
							button[i].On = TRUE;
						}
						else
						{
							button[i].On = FALSE;
						}
					}
					if (((fire.x - 50 <= red_door.x + 40 && fire.x - 50 >= red_door.x) || (fire.x <= red_door.x + 40 && fire.x >= red_door.x)) && ((fire.y - 80 >= red_door.y && fire.y - 80 <= red_door.y + 100) || (fire.y >= red_door.y - 100 && fire.y <= red_door.y + 100)))
					{
						red_door.On = TRUE;
					}
					else if (fire.x >= red_door.x && fire.x - 50 <= red_door.x && ((fire.y - 80 >= red_door.y && fire.y - 80 <= red_door.y) || (fire.y >= red_door.y + 100 && fire.y <= red_door.y + 100)))
					{
						red_door.On = TRUE;
					}
					else
					{
						red_door.On = FALSE;
					}
					if (((water.x - 50 <= blue_door.x + 40 && water.x - 50 >= blue_door.x) || (water.x <= blue_door.x + 40 && water.x >= blue_door.x)) && ((water.y - 80 >= blue_door.y && water.y - 80 <= blue_door.y) || (water.y >= blue_door.y + 100 && water.y <= blue_door.y + 100)))
					{
						blue_door.On = TRUE;
					}
					else if (water.x >= blue_door.x && water.x - 50 <= blue_door.x && ((water.y - 80 >= blue_door.y && water.y - 80 <= blue_door.y) || (water.y >= blue_door.y + 100 && water.y <= blue_door.y + 100)))
					{
						blue_door.On = TRUE;
					}
					else
					{
						blue_door.On = FALSE;
					}
					if (blue_door_open &&red_door_open&&red_total == 0 && blue_total == 0 && stage > 0)
					{
						stair = TRUE;
					}*/
			}
			break;

		case 2:				// 캐릭터 프레임
			if (++fire.Frame >= 8)	fire.Frame = 0;
			if (++water.Frame >= 8) water.Frame = 0;
			break;

		case 3:				// 보석, 함정, 사망, 문 애니메이션 처리와 스테이지 클리어 조건 충족시 후처리(Next 윈도우와 배경처리)
			/*
			blue_count = 0;
			red_count = 0;

			//// 파랑 보석 애니메이션
			//for (auto& bj : currentStage.Blue_Jewel) {
			//	if (bj.second.On) {
			//		blue_count += 1;
			//		bj.second.image_x += 29;
			//		if (bj.second.image_x == 1160)
			//		{
			//			bj.second.image_x = 0;
			//		}
			//	}
			//}
			//// 빨강 보석 애니메이션
			//for (auto& rj : currentStage.Red_Jewel) {
			//	if (rj.second.On) {
			//		red_count += 1;
			//		rj.second.image_x += 28;
			//		if (rj.second.image_x == 1120)
			//		{
			//			rj.second.image_x = 0;
			//		}
			//	}
			//}

			currentStage.red_total = red_count;
			currentStage.blue_total = blue_count;

			// 함정(빨강, 파랑, 초록 우물)의 애니메이션
			for (int i = 0; i < 90; i++) // 0~19까지 파랑 가운데 물 20~29까지 파랑 왼.오 30~49까지 빨강 가운데 물 50~59까지 빨강물 왼.오 60~79 초록 가운데 80~89 초록왼.오
			{
				if (currentStage.Trap[i].GetVisible())
				{
					if (i < 20)
					{
						currentStage.Trap[i].image_x += 23;
						if (currentStage.Trap[i].image_x == 230)
						{
							currentStage.Trap[i].image_x = 0;
						}
					}
					if (i >= 20 && i < 30)
					{
						currentStage.Trap[i].image_x += 23;
						if (currentStage.Trap[i].image_x == 345)
						{
							currentStage.Trap[i].image_x = 0;
						}
					}
					if (i >= 30 && i < 50)
					{
						currentStage.Trap[i].image_x += 21;
						if (currentStage.Trap[i].image_x == 315)
						{
							currentStage.Trap[i].image_x = 0;
						}
					}
					if (i >= 50 && i < 60)
					{
						currentStage.Trap[i].image_x += 24;
						if (currentStage.Trap[i].image_x == 360)
						{
							currentStage.Trap[i].image_x = 0;
						}
					}
					if (i >= 60 && i < 80)
					{
						currentStage.Trap[i].image_x += 23;
						if (currentStage.Trap[i].image_x == 345)
						{
							currentStage.Trap[i].image_x = 0;
						}
					}
					if (i >= 80 && i < 90)
					{
						currentStage.Trap[i].image_x += 24;
						if (currentStage.Trap[i].image_x == 360)
						{
							currentStage.Trap[i].image_x = 0;
						}
					}
				}
			}

			// Fire bot, Water girl 중 1명이라도 사망 && 현재 Stage가 1 이상인 경우 (Stage 0은 Title 화면)
			if (currentStage.Die.GetVisible() && currentStage.stage > 0)
			{
				// 캐릭터 (Fire, Water 공통) 사망시 나오는 연기 애니메이션
				currentStage.Die.image_x += 159;
				if (currentStage.Die.image_x == 7950)
				{
					back = TRUE;
					currentStage.Die.SetVisible(FALSE);
					currentStage.Die.image_x = 0;

					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					KillTimer(hWnd, 3);
					KillTimer(hWnd, 4);
					KillTimer(hWnd, 5);

					retry_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 300, 500, 200, 100, hWnd, (HMENU)IDC_BUTTON2, g_hInst, NULL);
					SendMessage(retry_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.retryimg));
					end_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 700, 500, 200, 100, hWnd, (HMENU)IDC_BUTTON3, g_hInst, NULL);
					SendMessage(end_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.endimg));
					mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
				}
			}

			// 스테이스 클리어 후 문으로 들어가는 애니메이션
			if (currentStage.stair)
			{
				stair_red_x += 50;
				stair_blue_x += 54;
				if (stair_red_x > 1250 && stair_blue_x > 1296)
				{
					currentStage.stair = FALSE;
					currentStage.red_door_open = FALSE;
					currentStage.blue_door_open = FALSE;
					currentStage.red_door.image_x = 0;
					currentStage.blue_door.image_x = 0;
					stair_red_x = 0;
					stair_blue_x = 0;
					currentStage.clear = TRUE;
				}
			}

			// 현재 스테이지 클리어 후 배경 처리(뒷 배경 어둡게)와 "Next" 버튼 윈도우 생성
			if (currentStage.clear)
			{
				if (currentStage.count == 0)
				{
					next_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 500, 400, 200, 100, hWnd, (HMENU)IDC_BUTTON4, g_hInst, NULL);
					SendMessage(next_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.clear_img));
					currentStage.count = 1;
				}
			}

			// "Stage_03() 에서 나오는 X축 이동 바" 의 동작 버튼 애니메이션 (버튼 42.16.PNG 파일 참고)
			for (int i = 0; i < 5; i++)
			{
				if (currentStage.button[i].GetVisible())
				{
					if (currentStage.button[i].image_y == 7)
					{
						currentStage.block[i].SetVisible(false);
					}
					else if (currentStage.button[i].image_y > 7)
					{
						currentStage.button[i].image_y -= 1;
					}
				}
				else
				{
					if (currentStage.button[i].image_y == 15)
					{
						currentStage.block[i].SetVisible(false);
					}
					else if (currentStage.button[i].image_y < 15)
					{
						currentStage.button[i].image_y += 1;
					}
				}
			}
			*/
			break;
		case 4:
			/*
			// Fire bot 출구 애니메이션
			if (currentStage.red_door.GetVisible())
			{
				if (currentStage.red_door.image_x == 1260)
				{
					currentStage.red_door_open = TRUE;
				}
				else if (currentStage.red_door.image_x < 1260)
				{
					currentStage.red_door.image_x += 60;
				}
			}
			else
			{
				currentStage.red_door_open = FALSE;
				if (currentStage.red_door.image_x > 0)
				{
					currentStage.red_door.image_x -= 60;
					//문 다열림
				}
			}

			// Water girl 출구 애니메이션
			if (currentStage.blue_door.GetVisible())
			{
				if (currentStage.blue_door.image_x == 1260)
				{
					currentStage.blue_door_open = TRUE;
				}
				else if (currentStage.blue_door.image_x < 1260)
				{
					currentStage.blue_door.image_x += 60;
				}
			}

			//문이 열리다가 중간에 이탈하면 다시 닫힘
			else
			{
				currentStage.blue_door_open = FALSE;
				if (currentStage.blue_door.image_x > 0)
				{
					currentStage.blue_door.image_x -= 60;
					//문 다열림
				}
			}

			// Stage_03() 에서 나오는 X축 이동 바 (block1.PNG 파일 참고)
			for (int i = 0; i < 5; i++)
			{
				if (currentStage.block[i].GetVisible())
				{
					if (currentStage.block[i].image_x > -40)
					{
						currentStage.block[i].image_x -= 2;
					}
				}
				else
				{
					if (currentStage.block[i].image_x < 0)
					{
						currentStage.block[i].image_x += 2;
					}
				}
			}
			*/
			break;

		case 5:
			//타임아웃시 할 동작
			/*
			if (--time == 0)
			{
				currentStage.time_over = TRUE;
				back = TRUE;
				retry_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 300, 500, 200, 100, hWnd, (HMENU)IDC_BUTTON2, g_hInst, NULL);
				SendMessage(retry_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.retryimg));
				end_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 700, 500, 200, 100, hWnd, (HMENU)IDC_BUTTON3, g_hInst, NULL);
				SendMessage(end_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.endimg));
				mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
				KillTimer(hWnd, 5);
			}
			*/
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		keybuffer[wParam] = TRUE;
		Loop(true);
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hDC);
		break;

	case WM_KEYUP:
		hDC = GetDC(hWnd);
		keybuffer[wParam] = FALSE;
		Loop(false);
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hDC);
		break;

	case WM_PAINT: {
		hDC = BeginPaint(hWnd, &ps);				
		hbitmap = CreateCompatibleBitmap(hDC, 1200, 800);
		memdc = CreateCompatibleDC(hDC);
		SelectObject(memdc, hbitmap);

		myImageMgr.DrawMap(&memdc, currentStage.stage, currentStage);
		//삭제
		if (stageIndex == 1) {
			currneClientNum += 1;
			if (currneClientNum == 4) {
				stageIndex += 1;
				currneClientNum = 3;
			}
		}
		
		if (currentStage.stage == 2) {		
			if (isArrow) {			
				selectRoleLeftArrow = CreateWindow(L"button", L"RoleSelect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 50, 280, 80, 41, hWnd, (HMENU)BTN_LEFT_ARROW, g_hInst, NULL);					
				selectRoleRightArrow = CreateWindow(L"button", L"RoleSelect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 330, 280, 80, 41, hWnd, (HMENU)BTN_RIGHT_ARROW, g_hInst, NULL);
				selectBtn = CreateWindow(L"button", L"RoleSelect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 500,620, 120, 45, hWnd, (HMENU)BTN_SELECT, g_hInst, NULL);
				SendMessage(selectRoleLeftArrow, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.leftArrow));
				SendMessage(selectRoleRightArrow, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.rightArrow));
				SendMessage(selectBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)myImageMgr.selectBtn));
				isArrow = false;
			}
		}



		if (currentStage.stage > 2) {
			myImageMgr.DrawPlayer(&memdc, 0, &water, currentStage);
			myImageMgr.DrawPlayer(&memdc, 0, &fire, currentStage);
			myImageMgr.DrawTimer(&memdc, time);

			for (auto& rj : currentStage.Red_Jewel) {
				if (rj.GetVisible())
					myImageMgr.Jewelry_red.Draw(memdc, rj.x, rj.y, rj.wid, rj.hei, rj.image_x, 0, 28, 24);
			}

			for (auto& bj : currentStage.Blue_Jewel) {
				if (bj.GetVisible())
					myImageMgr.Jewelry_blue.Draw(memdc, bj.x, bj.y, bj.wid, bj.hei, bj.image_x, 0, 28, 24);
			}
		}

		// 사망시 연기 Anim
		if (currentStage.Die.GetVisible())	myImageMgr.die.Draw(memdc, currentStage.Die.x, currentStage.Die.y, 100, 100, currentStage.Die.image_x, currentStage.Die.image_y, 159, 89);

		if (currentStage.clear)	back = TRUE;

		if (currentStage.time_over) myImageMgr.timeout.Draw(memdc, 400, 200, 400, 250, 0, 0, 486, 286);

		if (currentStage.stair)
		{
			myImageMgr.red_stair.Draw(memdc, currentStage.red_door.x, currentStage.red_door.y + 30, 50, 80, stair_red_x, 0, 50, 73);
			myImageMgr.blue_stair.Draw(memdc, currentStage.blue_door.x, currentStage.red_door.y + 30, 50, 80, stair_blue_x, 0, 54, 77);
			fire.on = FALSE;
			water.on = FALSE;
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

		BitBlt(hDC, 0, 0, 1200, 800, memdc, 0, 0, SRCCOPY);

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


		DeleteDC(hDC);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
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
