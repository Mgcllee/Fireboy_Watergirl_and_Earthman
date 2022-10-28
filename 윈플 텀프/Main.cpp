#pragma once
#include"stdafx.h"
#include "ImageMgr.h"
#include "StageMgr.h"

HINSTANCE g_hInst;
ImageMgr myImageMgr;
StageMgr myStageMgr;

int stageIndex = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	LPCTSTR IpszClass = L"fire boy and water girl";
	LPCTSTR IpszWindowName = L"window programming";

	g_hInst = hInstance;

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

	myImageMgr.LoadImages();

	hWnd = CreateWindow(IpszClass, IpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1200, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static HDC memdc;
	static HBITMAP hbitmap;
	HBRUSH hBrush, oldBrush;

	static HWND start_button, retry_button, end_button, next_button;
	static BOOL back = FALSE;
	static int time = 300;
	int blue_count = 0;
	int red_count = 0;
	static int stair_red_x = 0, stair_blue_x = 0;

	Stage currentStage = myStageMgr.getStage(stageIndex);

	switch (uMsg) {
	case WM_CREATE: {
		start_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 450, 600, 158, 60, hWnd, (HMENU)IDC_BUTTON1, g_hInst, NULL);
		SendMessage(start_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.buttonimg));

		// LoadSound(hWnd);

		SetTimer(hWnd, 1, 30, NULL);
		SetTimer(hWnd, 2, 100, NULL);
		SetTimer(hWnd, 3, 50, NULL);
		SetTimer(hWnd, 4, 50, NULL);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:	// First Start
			/*currentStage.stage = 1;
			currentStage.Stage_1(0);*/
			stageIndex++;
			SetTimer(hWnd, 5, 1000, NULL);
			LoadSound(hWnd);
			DestroyWindow(start_button);
			break;
		case IDC_BUTTON2:	// Restart Game
			time = 300;
			fire.on = TRUE;
			water.on = TRUE;

			switch (currentStage.stage)
			{
			case 1:
				currentStage.Stage_1(0);
				break;
			case 2:
				currentStage.Stage_2(0);
				break;
			case 3:
				currentStage.Stage_3(0);
				break;
			}
			LoadSound(hWnd);

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
		case IDC_BUTTON3:	// Game Quit
			back = FALSE;
			currentStage.time_over = FALSE;
			DestroyWindow(retry_button);
			DestroyWindow(end_button);
			PostQuitMessage(0);
			DeleteObject(hbitmap);
			DeleteDC(memdc);
			break;
		case IDC_BUTTON4:	// Next Stage
			back = FALSE;
			currentStage.clear = FALSE;
			fire.on = TRUE;
			water.on = TRUE;
			currentStage.stage += 1;
			time = 300;
			switch (currentStage.stage)
			{
			case 1:
				currentStage.Stage_1(0);
				break;
			case 2:
				currentStage.Stage_2(0);
				break;
			case 3:
				currentStage.Stage_3(0);
				break;
			}
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
			LoadSound(hWnd);
			DestroyWindow(next_button);
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_TIMER:			// m/s Timer Function
		switch (wParam) {
		case 1:				// Ä³¸¯ÅÍ ÀÌµ¿°ú Ãæµ¹Ã¼Å©
			currentStage.Wid_Move();
			currentStage.Jump();
			currentStage.Foot();
			currentStage.Push();



			//if (stage > 0)
			//{
			//	for (int i = 0; i < 20; i++)
			//	{
			//		if (i >= 10)
			//		{
			//			if (((fire.x - 50 <= Jewelry[i].x + 28 && fire.x - 50 >= Jewelry[i].x) || (fire.x <= Jewelry[i].x + 28 && fire.x >= Jewelry[i].x)) && ((fire.y - 80 >= Jewelry[i].y && fire.y - 80 <= Jewelry[i].y + 25) || (fire.y >= Jewelry[i].y && fire.y <= Jewelry[i].y + 25)))
			//			{
			//				Jewelry[i].On = FALSE;
			//			}
			//			else if (fire.x - 50 <= Jewelry[i].x && fire.x >= Jewelry[i].x && fire.y - 80 <= Jewelry[i].y && fire.y >= Jewelry[i].y)
			//			{
			//				Jewelry[i].On = FALSE;
			//			}
			//		}
			//		if (i < 10)
			//		{
			//			if (((water.x - 50 <= Jewelry[i].x + 28 && water.x - 50 >= Jewelry[i].x) || (water.x <= Jewelry[i].x + 28 && water.x >= Jewelry[i].x)) && ((water.y - 80 >= Jewelry[i].y && water.y - 80 <= Jewelry[i].y + 25) || (water.y >= Jewelry[i].y && water.y <= Jewelry[i].y + 25)))
			//			{
			//				Jewelry[i].On = FALSE;
			//			}
			//			else if (water.x - 50 <= Jewelry[i].x && water.x >= Jewelry[i].x && water.y - 80 <= Jewelry[i].y && water.y >= Jewelry[i].y)
			//			{
			//				Jewelry[i].On = FALSE;
			//			}
			//		}
			//	}
			//	for (int i = 0; i < 90; i++)	// 0~19±îÁö ÆÄ¶û °¡¿îµ¥ 20~24 ÆÄ¶û¿Þ 25~29ÆÄ¶û¿À 30~49 »¡°­ °¡¿îµ¥ 50~54~»¡°­¿Þ 55~59»¡°­¿À 60~79ÃÊ·Ï °¡¿îµ¥ 80~84ÃÊ·Ï¿Þ 85~89ÃÊ·Ï¿À
			//	{
			//		if (i < 30)
			//		{
			//			if (((fire.x - 50 <= Trap[i].x + 28 && fire.x - 50 >= Trap[i].x) || (fire.x <= Trap[i].x + 28 && fire.x >= Trap[i].x)) && ((fire.y - 80 >= Trap[i].y && fire.y - 80 <= Trap[i].y + 25) || (fire.y >= Trap[i].y && fire.y <= Trap[i].y + 25)))
			//			{
			//				fire.on = FALSE;
			//				Die.On = TRUE;
			//				Die.x = Trap[i].x;
			//				Die.y = Trap[i].y-100;
			//			}
			//		}
			//		if (i >= 30 && i < 60)
			//		{
			//			if (((water.x - 50 <= Trap[i].x + 25 && water.x - 50 >= Trap[i].x) || (water.x <= Trap[i].x + 25 && water.x >= Trap[i].x)) && ((water.y - 80 >= Trap[i].y && water.y - 80 <= Trap[i].y + 25) || (water.y >= Trap[i].y && water.y <= Trap[i].y + 25)))
			//			{
			//				water.on = FALSE;
			//				Die.On = TRUE;
			//				Die.x = Trap[i].x;
			//				Die.y = Trap[i].y-100;
			//			}
			//		}
			//		if (i >= 60 && i < 90)
			//		{
			//			if (((fire.x - 50 <= Trap[i].x + 25 && fire.x - 50 >= Trap[i].x) || (fire.x <= Trap[i].x + 25 && fire.x >= Trap[i].x)) && ((fire.y - 80 >= Trap[i].y && fire.y - 80 <= Trap[i].y + 25) || (fire.y >= Trap[i].y && fire.y <= Trap[i].y + 25)))
			//			{
			//				fire.on = FALSE;
			//				Die.On = TRUE;
			//				Die.x = Trap[i].x;
			//				Die.y = Trap[i].y-100;
			//			}
			//			if (((water.x - 50 <= Trap[i].x + 25 && water.x - 50 >= Trap[i].x) || (water.x <= Trap[i].x + 25 && water.x >= Trap[i].x)) && ((water.y - 80 >= Trap[i].y && water.y - 80 <= Trap[i].y + 25) || (water.y >= Trap[i].y && water.y <= Trap[i].y + 25)))
			//			{
			//				water.on = FALSE;
			//				Die.On = TRUE;
			//				Die.x = Trap[i].x;
			//				Die.y = Trap[i].y-100;
			//			}
			//		}
			//	}
			//	for (int i = 0; i < 5; i++)
			//	{
			//		if (((fire.x - 50 <= button[i].x + 40 && fire.x - 50 >= button[i].x) || (fire.x <= button[i].x + 40 && fire.x >= button[i].x)) && ((fire.y - 80 >= button[i].y - button[i].image_y && fire.y - 80 <= button[i].y) || (fire.y >= button[i].y - button[i].image_y && fire.y <= button[i].y)))
			//		{
			//			button[i].On = TRUE;
			//		}
			//		else if (((water.x - 50 <= button[i].x + 40 && water.x - 50 >= button[i].x) || (water.x <= button[i].x + 40 && water.x >= button[i].x)) && ((water.y - 80 >= button[i].y - button[i].image_y && water.y - 80 <= button[i].y) || (water.y >= button[i].y - button[i].image_y && water.y <= button[i].y)))
			//		{
			//			button[i].On = TRUE;
			//		}
			//		else if (fire.x >= button[i].x && fire.x - 50 <= button[i].x && ((fire.y - 80 >= button[i].y && fire.y - 80 <= button[i].y - -button[i].image_y) || (fire.y >= button[i].y - button[i].image_y && fire.y <= button[i].y - -button[i].image_y)))
			//		{
			//			button[i].On = TRUE;
			//		}
			//		else if (water.x >= button[i].x && water.x - 50 <= button[i].x && ((water.y - 80 >= button[i].y && water.y - 80 <= button[i].y - -button[i].image_y) || (water.y >= button[i].y - button[i].image_y && water.y <= button[i].y - -button[i].image_y)))
			//		{
			//			button[i].On = TRUE;
			//		}
			//		else
			//		{
			//			button[i].On = FALSE;
			//		}
			//	}
			//	for (int i = 0; i < 5; i++)
			//	{
			//		if (((Rt.x - 50 <= button[i].x + 40 && Rt.x - 50 >= button[i].x) || (Rt.x <= button[i].x + 40 && Rt.x >= button[i].x)) && ((Rt.y - 50 >= button[i].y - button[i].image_y && Rt.y - 50 <= button[i].y) || (Rt.y >= button[i].y - button[i].image_y && Rt.y <= button[i].y)))
			//		{
			//			button[i].On = TRUE;
			//		}
			//		else if (Rt.x >= button[i].x && Rt.x - 50 <= button[i].x && ((Rt.y - 80 >= button[i].y && Rt.y - 80 <= button[i].y - -button[i].image_y) || (Rt.y >= button[i].y - button[i].image_y && Rt.y <= button[i].y - -button[i].image_y)))
			//		{
			//			button[i].On = TRUE;
			//		}
			//		else
			//		{
			//			button[i].On = FALSE;
			//		}
			//	}
			//	if (((fire.x - 50 <= red_door.x + 40 && fire.x - 50 >= red_door.x) || (fire.x <= red_door.x + 40 && fire.x >= red_door.x)) && ((fire.y - 80 >= red_door.y && fire.y - 80 <= red_door.y + 100) || (fire.y >= red_door.y - 100 && fire.y <= red_door.y + 100)))
			//	{
			//		red_door.On = TRUE;
			//	}
			//	else if (fire.x >= red_door.x && fire.x - 50 <= red_door.x && ((fire.y - 80 >= red_door.y && fire.y - 80 <= red_door.y) || (fire.y >= red_door.y + 100 && fire.y <= red_door.y + 100)))
			//	{
			//		red_door.On = TRUE;
			//	}
			//	else
			//	{
			//		red_door.On = FALSE;
			//	}
			//	if (((water.x - 50 <= blue_door.x + 40 && water.x - 50 >= blue_door.x) || (water.x <= blue_door.x + 40 && water.x >= blue_door.x)) && ((water.y - 80 >= blue_door.y && water.y - 80 <= blue_door.y) || (water.y >= blue_door.y + 100 && water.y <= blue_door.y + 100)))
			//	{
			//		blue_door.On = TRUE;
			//	}
			//	else if (water.x >= blue_door.x && water.x - 50 <= blue_door.x && ((water.y - 80 >= blue_door.y && water.y - 80 <= blue_door.y) || (water.y >= blue_door.y + 100 && water.y <= blue_door.y + 100)))
			//	{
			//		blue_door.On = TRUE;
			//	}
			//	else
			//	{
			//		blue_door.On = FALSE;
			//	}
			//	if (blue_door_open &&red_door_open&&red_total == 0 && blue_total == 0 && stage > 0)
			//	{
			//		stair = TRUE;
			//	}
			//}
			break;

		case 2:				// Ä³¸¯ÅÍ ÇÁ·¹ÀÓ
			if (++fire.Frame >= 8) fire.Frame = 0;
			if (++water.Frame >= 8) water.Frame = 0;
			break;

		case 3:				// 
			blue_count = 0;
			red_count = 0;

			for (auto& bj : currentStage.Blue_Jewel) {
				if (bj.second.On) {
					blue_count += 1;
					bj.second.image_x += 29;
					if (bj.second.image_x == 1160)
					{
						bj.second.image_x = 0;
					}
				}
			}

			for (auto& rj : currentStage.Red_Jewel) {
				if (rj.second.On) {
					red_count += 1;
					rj.second.image_x += 28;
					if (rj.second.image_x == 1120)
					{
						rj.second.image_x = 0;
					}
				}
			}
			currentStage.red_total = red_count;
			currentStage.blue_total = blue_count;

			for (int i = 0; i < 90; i++) // 0~19±îÁö ÆÄ¶û °¡¿îµ¥ ¹° 20~29±îÁö ÆÄ¶û ¿Þ.¿À 30~49±îÁö »¡°­ °¡¿îµ¥ ¹° 50~59±îÁö »¡°­¹° ¿Þ.¿À 60~79 ÃÊ·Ï °¡¿îµ¥ 80~89 ÃÊ·Ï¿Þ.¿À
			{
				if (currentStage.Trap[i].On)
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

			if (currentStage.Die.On && currentStage.stage > 0)
			{
				currentStage.Die.image_x += 159;
				if (currentStage.Die.image_x == 7950)
				{
					back = TRUE;
					currentStage.Die.On = FALSE;
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

			if (currentStage.clear)
			{
				if (currentStage.count == 0)
				{
					next_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 500, 400, 200, 100, hWnd, (HMENU)IDC_BUTTON4, g_hInst, NULL);
					SendMessage(next_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)myImageMgr.clear_img));
					currentStage.count = 1;
				}
			}
			for (int i = 0; i < 5; i++)
			{
				if (currentStage.button[i].On)
				{
					if (currentStage.button[i].image_y == 7)
					{
						currentStage.block[i].On = TRUE;
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
						currentStage.block[i].On = FALSE;
					}
					else if (currentStage.button[i].image_y < 15)
					{
						currentStage.button[i].image_y += 1;
					}
				}
			}
			break;
		case 4:
			// ÆÄ¶û, »¡°­ Ãâ±¸ ¾Ö´Ï¸ÞÀÌ¼Ç
			if (currentStage.red_door.On)
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
					//¹® ´Ù¿­¸²
				}
			}
			if (currentStage.blue_door.On)
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
			else//¹®ÀÌ ¿­¸®´Ù°¡ Áß°£¿¡ ÀÌÅ»ÇÏ¸é ´Ù½Ã ´ÝÈû
			{
				currentStage.blue_door_open = FALSE;
				if (currentStage.blue_door.image_x > 0)
				{
					currentStage.blue_door.image_x -= 60;
					//¹® ´Ù¿­¸²
				}
			}

			for (int i = 0; i < 5; i++)
			{
				if (currentStage.block[i].On)
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
			break;

		case 5:				//Å¸ÀÓ¾Æ¿ô½Ã ÇÒ µ¿ÀÛ
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
		myImageMgr.DrawPlayer(&memdc, 0, &water, currentStage);
		myImageMgr.DrawPlayer(&memdc, 0, &fire, currentStage);
		myImageMgr.DrawTimer(&memdc, time);

		// »ç¸Á½Ã ¿¬±â Anim
		if (currentStage.Die.On)	myImageMgr.die.Draw(memdc, currentStage.Die.x, currentStage.Die.y, 100, 100, currentStage.Die.image_x, currentStage.Die.image_y, 159, 89);

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

		if (currentStage.clear == FALSE && back == FALSE)
		{
			for (auto& bj : currentStage.Blue_Jewel) {
				myImageMgr.Jewelry_blue.Draw(hDC, bj.second.x, bj.second.y, 28, 25, bj.second.image_x, 0, 28, 24);
			}

			for (auto& rj : currentStage.Red_Jewel) {
				myImageMgr.Jewelry_red.Draw(hDC, rj.second.x, rj.second.y, 28, 25, rj.second.image_x, 0, 28, 24);
			}

			for (int i = 0; i < 90; i++) // 0~19±îÁö ÆÄ¶û °¡¿îµ¥ ¹° 20~29±îÁö ÆÄ¶û ¿Þ.¿À 30~49±îÁö »¡°­ °¡¿îµ¥ ¹° 50~59±îÁö »¡°­¹° ¿Þ.¿À 60~79 ÃÊ·Ï °¡¿îµ¥ 80~89 ÃÊ·Ï¿Þ.¿À
			{
				if (currentStage.Trap[i].On)
				{
					if (i < 20)
					{
						myImageMgr.blue_water_center.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 23, 14);
					}
					if (i >= 20 && i < 25)
					{
						myImageMgr.blue_water_left.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 23, 23);
					}
					if (i >= 25 && i < 30)
					{
						myImageMgr.blue_water_right.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 23, 23);
					}
					if (i >= 30 && i < 50)
					{
						myImageMgr.red_water_center.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 21, 14);
					}
					if (i >= 50 && i < 55)
					{
						myImageMgr.red_water_left.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 24, 25);
					}
					if (i >= 55 && i < 60)
					{
						myImageMgr.red_water_right.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 24, 25);
					}
					if (i >= 60 && i < 80)
					{
						myImageMgr.green_water_center.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 23, 14);
					}
					if (i >= 80 && i < 85)
					{
						myImageMgr.green_water_left.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 24, 25);
					}
					if (i >= 85 && i < 90)
					{
						myImageMgr.green_water_right.Draw(hDC, currentStage.Trap[i].x, currentStage.Trap[i].y, 25, 25, currentStage.Trap[i].image_x, currentStage.Trap[i].image_y, 24, 25);
					}
				}
			}

		}

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
