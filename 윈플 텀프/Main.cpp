#include"Main_head.h"
#pragma comment(lib,"winmm.lib")
#pragma comment (lib, "msimg32.lib")

#define IDC_BUTTON1 100
#define IDC_BUTTON2 200
#define IDC_BUTTON3 300
#define IDC_BUTTON4 350
HINSTANCE g_hInst;
LPCTSTR IpszClass = L"fire boy and water girl";
LPCTSTR IpszWindowName = L"window programming";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void LoadSound(HWND hWnd);
DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
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

MCI_OPEN_PARMS      mciOpenParms;
MCI_PLAY_PARMS       mciPlayParms;
MCI_STATUS_PARMS   mciStatus;

UINT wDeviceID = 0;

BOOL playsound = FALSE;
static int stage = 0;
static BOOL clear = FALSE;
static int average = 0;
static BOOL time_over = FALSE;
static BOOL red_door_open = FALSE, blue_door_open = FALSE;
static int count = 0;
static int red_total = 0;
static int blue_total = 0;
static BOOL stair =FALSE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static HDC memdc;
	static HBITMAP hbitmap;
	HBRUSH hBrush, oldBrush;
	static CImage robby, buttonimg, stage1, Jewelry_blue, Jewelry_red, red_water_center, red_water_left, red_water_right, blue_water_center, blue_water_left, blue_water_right, green_water_center, green_water_left, green_water_right, die, retryimg, endimg, backimg, door_red, door_blue, button_img, clear_img,red_stair,blue_stair,rect;
	static CImage zero, one, two, three, four, five, six, seven, eight, nine, clock, timeout;
	static CImage block_w, block_h,foot_block;
	static CImage Fire[5], Water[5];
	static int F_Frame = 0, W_Frame = 0;
	static HWND start_button, retry_button, end_button, next_button;
	static int mx, my;
	static BOOL back = FALSE;
	static int time = 300;
	int blue_count = 0;
	int red_count = 0;
	static int stair_red_x=0, stair_blue_x = 0;
	TCHAR text[30];
	switch (uMsg) {
	case WM_CREATE:
	{
		robby.Load(L"Resource\\로비 이미지 초안 2.png");
		buttonimg.Load(L"Resource\\플레이 버튼 158.60.png");
		stage1.Load(L"Resource\\맵 기본.png");
		Jewelry_blue.Load(L"Resource\\파랑 보석 28.24.png");
		Jewelry_red.Load(L"Resource\\빨강 보석 29.24.png");
		red_water_center.Load(L"Resource\\빨강 가운데물 21.14.png");
		red_water_left.Load(L"Resource\\빨강 왼쪽물 24.25.png");
		red_water_right.Load(L"Resource\\빨강 오른쪽물 24.25.png");
		blue_water_center.Load(L"Resource\\파랑 가운데물 23.14.png");
		blue_water_left.Load(L"Resource\\파랑 왼쪽물 23.23.png");
		blue_water_right.Load(L"Resource\\파랑 오른쪽물 23.23.png");
		green_water_center.Load(L"Resource\\초록 가운데물 23.14.png");
		green_water_left.Load(L"Resource\\초록 왼쪽물 24.25.png");
		green_water_right.Load(L"Resource\\초록 오른물 24.25.png");
		die.Load(L"Resource\\죽었을 때 159.89.png");
		endimg.Load(L"Resource\\end.png");
		retryimg.Load(L"Resource\\retry.png");
		backimg.Load(L"Resource\\dieimg.png");
		door_red.Load(L"Resource\\빨강 문 60.104.png");
		door_blue.Load(L"Resource\\파랑 문 60.104.png");
		button_img.Load(L"Resource\\버튼 42.16.png");
		red_stair.Load(L"Resource\\빨강 계단 50.73.png");
		blue_stair.Load(L"Resource\\파랑 계단 54.77.png");
		clear_img.Load(L"Resource\\클리어.png");
		zero.Load(L"Resource\\0.png");
		one.Load(L"Resource\\1.png");
		two.Load(L"Resource\\2.png");
		three.Load(L"Resource\\3.png");
		four.Load(L"Resource\\4.png");
		five.Load(L"Resource\\5.png");
		six.Load(L"Resource\\6.png");
		seven.Load(L"Resource\\7.png");
		eight.Load(L"Resource\\8.png");
		nine.Load(L"Resource\\9.png");
		clock.Load(L"Resource\\시계.png");
		timeout.Load(L"Resource\\타임아웃.png");
		Fire[0].Load(L"Resource\\빨강 정지 215.411.png");        // 정지
		Fire[1].Load(L"Resource\\빨강 정지 215.411.png");        // 상승
		Fire[2].Load(L"Resource\\빨강 오른쪽 342.271.png");    // 우측
		Fire[3].Load(L"Resource\\빨강 하강 215.411.png");        // 하단
		Fire[4].Load(L"Resource\\빨강 왼쪽 342.271.png");        // 좌측

		Water[0].Load(L"Resource\\파랑 정지 207.480.png");        // 정지
		Water[1].Load(L"Resource\\파랑 정지 207.480.png");        // 상승
		Water[2].Load(L"Resource\\파랑 오른쪽 376.480.png");        // 우측
		Water[3].Load(L"Resource\\파랑 하강 203.553.png");        // 하단
		Water[4].Load(L"Resource\\파랑 왼쪽 376.480.png");        // 좌측

		block_w.Load(L"Resource\\block1.png");
		block_h.Load(L"Resource\\block2.png");
		foot_block.Load(L"Resource\\발판.png");
		rect.Load(L"Resource\\상자 40.40.png");
	}
	if (stage == 0)
	{
		start_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 450, 600, 158, 60, hWnd, (HMENU)IDC_BUTTON1, g_hInst, NULL);
		SendMessage(start_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)buttonimg));
	}
	playsound = TRUE;
	if (playsound)
	{
		LoadSound(hWnd);
	}
	for (int i = 0; i < 20; i++)
	{
		if (Jewelry[i].On)
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
	}
	red_total = red_count;
	blue_total = blue_count;
	
SetTimer(hWnd, 1, 30, NULL);
	SetTimer(hWnd, 2, 100, NULL);
	SetTimer(hWnd, 3, 50, NULL);
	SetTimer(hWnd, 4, 50, NULL);
	break;
	//case WM_MOUSEMOVE:
	//	mx = LOWORD(lParam);
	//	my = HIWORD(lParam);
	//	InvalidateRect(hWnd, NULL, FALSE);
	//	break;
	//case WM_RBUTTONUP:
	//	clear = TRUE;
	//	InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			stage = 1;
			Stage_1(0);
			SetTimer(hWnd, 5, 1000, NULL);
			LoadSound(hWnd);
			DestroyWindow(start_button);
			break;
		case IDC_BUTTON2://해당 스테이지 초기화 후 재시작
			time = 300;
			fire.on = TRUE;
			water.on = TRUE;
			switch (stage)
			{
			case 1:
				Stage_1(0);
				break;
			case 2:
				Stage_2(0);
				break;
			case 3:
				Stage_3(0);
				break;
			case 4:
				break;
			case 5:
				break;
			}
			for (int i = 0; i < 20; i++)
			{
				if (Jewelry[i].On)
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
			}
			red_total = red_count;
			blue_total = blue_count;
		SetTimer(hWnd, 1, 30, NULL);
			SetTimer(hWnd, 2, 100, NULL);
			SetTimer(hWnd, 3, 50, NULL);
			SetTimer(hWnd, 4, 50, NULL);
			SetTimer(hWnd, 5, 1000, NULL);
			back = FALSE;
			time_over = FALSE;
			LoadSound(hWnd);
			DestroyWindow(retry_button);
			DestroyWindow(end_button);
			break;
		case IDC_BUTTON3://게임 종료
			back = FALSE;
			time_over = FALSE;
			DestroyWindow(retry_button);
			DestroyWindow(end_button);
			PostQuitMessage(0);
			DeleteObject(hbitmap);
			DeleteDC(memdc);
			break;
		case IDC_BUTTON4:
			back = FALSE;
			clear = FALSE;
			fire.on = TRUE;
			water.on = TRUE;
			stage += 1;
			time = 300;
			switch (stage)
			{
			case 1:
				Stage_1(0);
				break;
			case 2:
				Stage_2(0);
				break;
			case 3:
				Stage_3(0);
				break;
			case 4:
				break;
			case 5:
				break;
			}
			for (int i = 0; i < 20; i++)
			{
				if (Jewelry[i].On)
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
			}
			red_total = red_count;
			blue_total = blue_count;
			count = 0;
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
	case WM_TIMER:
		switch (wParam) {
		case 1:
            Wid_Move();
            Jump();
            Foot();
            Push();
			if (stage > 0)
			{
				for (int i = 0; i < 20; i++)
				{
					if (i >= 10)
					{
						if (((fire.x - 50 <= Jewelry[i].x + 28 && fire.x - 50 >= Jewelry[i].x) || (fire.x <= Jewelry[i].x + 28 && fire.x >= Jewelry[i].x)) && ((fire.y - 80 >= Jewelry[i].y && fire.y - 80 <= Jewelry[i].y + 25) || (fire.y >= Jewelry[i].y && fire.y <= Jewelry[i].y + 25)))
						{
							Jewelry[i].On = FALSE;
						}
						else if (fire.x - 50 <= Jewelry[i].x && fire.x >= Jewelry[i].x && fire.y - 80 <= Jewelry[i].y && fire.y >= Jewelry[i].y)
						{
							Jewelry[i].On = FALSE;
						}
					}

					if (i < 10)
					{
						if (((water.x - 50 <= Jewelry[i].x + 28 && water.x - 50 >= Jewelry[i].x) || (water.x <= Jewelry[i].x + 28 && water.x >= Jewelry[i].x)) && ((water.y - 80 >= Jewelry[i].y && water.y - 80 <= Jewelry[i].y + 25) || (water.y >= Jewelry[i].y && water.y <= Jewelry[i].y + 25)))
						{
							Jewelry[i].On = FALSE;
						}
						else if (water.x - 50 <= Jewelry[i].x && water.x >= Jewelry[i].x && water.y - 80 <= Jewelry[i].y && water.y >= Jewelry[i].y)
						{
							Jewelry[i].On = FALSE;
						}
					}
				}

				for (int i = 0; i < 90; i++)	// 0~19까지 파랑 가운데 20~24 파랑왼 25~29파랑오 30~49 빨강 가운데 50~54~빨강왼 55~59빨강오 60~79초록 가운데 80~84초록왼 85~89초록오
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
				}

				for (int i = 0; i < 5; i++)
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
				}
			}
			break;
		case 2:
			++F_Frame;
			if (F_Frame >= 8) {
				F_Frame = 0;
			}
			++W_Frame;
			if (W_Frame >= 8) {
				W_Frame = 0;
			}
			break;
		case 3:
			blue_count = 0;
			red_count = 0;
			for (int i = 0; i < 20; i++)
			{
				if (Jewelry[i].On)
				{
					if (i < 10)
					{
						red_count++;
						Jewelry[i].image_x += 28;
						if (Jewelry[i].image_x == 1120)
						{
							Jewelry[i].image_x = 0;
						}
					}

					if (i >= 10 && i<20)
					{
						blue_count++;
						Jewelry[i].image_x += 29;
						if (Jewelry[i].image_x == 1160)
						{
							Jewelry[i].image_x = 0;
						}
					}
				}
			}
			red_total = red_count;
			blue_total = blue_count;
			for (int i = 0; i < 90; i++) // 0~19까지 파랑 가운데 물 20~29까지 파랑 왼.오 30~49까지 빨강 가운데 물 50~59까지 빨강물 왼.오 60~79 초록 가운데 80~89 초록왼.오
			{
				if (Trap[i].On)
				{
					if (i < 20)
					{
						Trap[i].image_x += 23;
						if (Trap[i].image_x == 230)
						{
							Trap[i].image_x = 0;
						}
					}
					if (i >= 20 && i < 30)
					{
						Trap[i].image_x += 23;
						if (Trap[i].image_x == 345)
						{
							Trap[i].image_x = 0;
						}
					}
					if (i >= 30 && i < 50)
					{
						Trap[i].image_x += 21;
						if (Trap[i].image_x == 315)
						{
							Trap[i].image_x = 0;
						}
					}
					if (i >= 50 && i < 60)
					{
						Trap[i].image_x += 24;
						if (Trap[i].image_x == 360)
						{
							Trap[i].image_x = 0;
						}
					}
					if (i >= 60 && i < 80)
					{
						Trap[i].image_x += 23;
						if (Trap[i].image_x == 345)
						{
							Trap[i].image_x = 0;
						}
					}
					if (i >= 80 && i < 90)
					{
						Trap[i].image_x += 24;
						if (Trap[i].image_x == 360)
						{
							Trap[i].image_x = 0;
						}
					}
				}
			}

			if (Die.On&&stage>0)
			{
				Die.image_x += 159;
				if (Die.image_x == 7950)
				{
					back = TRUE;
					Die.On = FALSE;
					Die.image_x = 0;
					
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					KillTimer(hWnd, 3);
					KillTimer(hWnd, 4);
					KillTimer(hWnd, 5);
					retry_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 300, 500, 200, 100, hWnd, (HMENU)IDC_BUTTON2, g_hInst, NULL);
					SendMessage(retry_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)retryimg));
					end_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 700, 500, 200, 100, hWnd, (HMENU)IDC_BUTTON3, g_hInst, NULL);
					SendMessage(end_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)endimg));
					mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
				}
			}

			if (stair)
			{
				stair_red_x += 50;
				stair_blue_x += 54;
				if (stair_red_x > 1250 && stair_blue_x>1296)
				{
					stair = FALSE;
					red_door_open = FALSE;
					blue_door_open = FALSE;
					red_door.image_x = 0;
					blue_door.image_x = 0;
					stair_red_x = 0;
					stair_blue_x = 0;
					clear = TRUE;
				}
			}

			if (clear)
			{
				if (count == 0)
				{
					next_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 500, 400, 200, 100, hWnd, (HMENU)IDC_BUTTON4, g_hInst, NULL);
					SendMessage(next_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)clear_img));
					count = 1;
				}
			}
			for (int i = 0; i < 5; i++)
			{
				if (button[i].On)
				{
					if (button[i].image_y == 7)
					{
						block[i].On = TRUE;
					}
					else if (button[i].image_y > 7)
					{
						button[i].image_y -= 1;
					}
				}
				else
				{
					if (button[i].image_y == 15)
					{
						block[i].On = FALSE;
					}
					else if (button[i].image_y < 15)
					{
						button[i].image_y += 1;
					}
				}
			}
			break;
		case 4:
			if (red_door.On)
			{
				if (red_door.image_x == 1260)
				{
					red_door_open = TRUE;
				}
				else if (red_door.image_x < 1260)
				{
					red_door.image_x += 60;
				}
			}
			else//문이 열리다가 중간에 이탈하면 다시 닫힘
			{
				red_door_open = FALSE;
				if (red_door.image_x > 0)
				{
					red_door.image_x -= 60;
					//문 다열림
				}
			}

			if (blue_door.On)
			{
				if (blue_door.image_x == 1260)
				{
					blue_door_open = TRUE;
				}
				else if (blue_door.image_x < 1260)
				{
					blue_door.image_x += 60;
				}
			}
			else//문이 열리다가 중간에 이탈하면 다시 닫힘
			{
				blue_door_open = FALSE;
				if (blue_door.image_x > 0)
				{
					blue_door.image_x -= 60;
					//문 다열림
				}
			}

			for (int i = 0; i < 5; i++)
			{
				if (block[i].On)
				{
					if (block[i].image_x > -40)
					{
						block[i].image_x -= 2;
					}
				}
				else
				{
					if (block[i].image_x < 0)
					{
						block[i].image_x += 2;
					}
				}
			}
			break;
		case 5:
			time--;
			if (time == 0)
			{
				//타임아웃시 할 동작
				time_over = TRUE;
				back = TRUE;
				retry_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 300, 500, 200, 100, hWnd, (HMENU)IDC_BUTTON2, g_hInst, NULL);
				SendMessage(retry_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)retryimg));
				end_button = CreateWindow(L"button", L"123123", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 700, 500, 200, 100, hWnd, (HMENU)IDC_BUTTON3, g_hInst, NULL);
				SendMessage(end_button, BM_SETIMAGE, 0, (LPARAM)((HBITMAP)endimg));
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
		Loop();
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_KEYUP:
		hDC = GetDC(hWnd);
		keybuffer[wParam] = FALSE;
		water.is_Move = FALSE;
		water.wid_a = 0;
		water.is_Speed_Down = TRUE;
		fire.is_Move = FALSE;
		fire.wid_a = 0;
		F_Frame = 0;
		W_Frame=0;
		fire.is_Speed_Down = TRUE;
		InvalidateRect(hWnd, NULL, FALSE);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		hbitmap = CreateCompatibleBitmap(hDC, 1200, 800);
		memdc = CreateCompatibleDC(hDC);
		SelectObject(memdc, hbitmap);
		if (stage == 0)
		{
			robby.Draw(memdc, 0, 0, 1190, 770, 0, 0, 640, 480);
		}
		else if (stage == 1)
		{
			stage1.Draw(memdc, 0, 0, 1190, 765, 0, 480 - average, 640, 480);

			for (int i = 0; i < 20 && Ft[i].x != NULL; ++i)
			{
				foot_block.Draw(memdc, Ft[i].x, Ft[i].y, Ft[i].wid, Ft[i].hei, 0, 0, 111, 23);
			}
			
		//	block_w.Draw(memdc, block[0].x+block[0].image_x, block[0].y, 100+block[0].image_x, 30, -block[0].image_x, 0, 83+block[0].image_x, 25);

			door_red.Draw(memdc, red_door.x, red_door.y, 60, 100, red_door.image_x, red_door.image_y, 60, 104);
			door_blue.Draw(memdc, blue_door.x, blue_door.y, 60, 100, blue_door.image_x, blue_door.image_y, 60, 104);
			//button_img.Draw(memdc, button[0].x, button[0].y - button[0].image_y, 40, button[0].image_y, 0, 0, button[0].image_x, button[0].image_y);
		

			if (fire.on)
			{
				if (fire.Down == TRUE && fire.dic == 0) {
					Fire[3].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0, 0, 215, 411);
				}
				else if (fire.dic == 0) {
					Fire[0].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0, 0, 215, 411);
				}
				else if (fire.dic == -1) {
					Fire[4].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0 + 342 * F_Frame, 0, 342, 271);
				}
				else if (fire.dic == 1) {
					Fire[2].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0 + 342 * F_Frame, 0, 342, 271);
				}
			}

			if (water.on)
			{
				if (water.Down == TRUE && water.dic == 0) {
					Water[3].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 140, 0, 0, 203, 553);
				}
				else if (water.dic == 0) {
					Water[0].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0, 0, 207, 400);
				}
				else if (water.dic == -1) {
					Water[4].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0 + 376 * W_Frame, 0, 376, 400);
				}
				else if (water.dic == 1) {
					Water[2].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0 + 376 * W_Frame, 0, 376, 400);
				}
			}
			clock.Draw(memdc, 525, 0, 150, 50, 0, 0, 154, 54);
			switch (time / 60) {
			case 0:
				zero.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 1:
				one.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 2:
				two.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 3:
				three.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 4:
				four.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 5:
				five.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 6:
				six.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 7:
				seven.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 8:
				eight.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 9:
				nine.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			}
			switch ((time % 60) / 10) {
			case 0:
				zero.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 1:
				one.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 2:
				two.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 3:
				three.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 4:
				four.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 5:
				five.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 6:
				six.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 7:
				seven.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 8:
				eight.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 9:
				nine.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			}
			switch ((time % 60) % 10) {
			case 0:
				zero.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 1:
				one.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 2:
				two.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 3:
				three.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 4:
				four.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 5:
				five.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 6:
				six.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 7:
				seven.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 8:
				eight.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 9:
				nine.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			}
		}
		else if (stage == 2)
		{
			stage1.Draw(memdc, 0, 0, 1190, 765, 0, 480 - average, 640, 480);

			for (int i = 0; i < 20 && Ft[i].x != NULL; ++i)
			{
				foot_block.Draw(memdc, Ft[i].x, Ft[i].y, Ft[i].wid, Ft[i].hei, 0, 0, 111, 23);
			}


			//	block_w.Draw(memdc, block[0].x+block[0].image_x, block[0].y, 100+block[0].image_x, 30, -block[0].image_x, 0, 83+block[0].image_x, 25);

			door_red.Draw(memdc, red_door.x, red_door.y, 60, 100, red_door.image_x, red_door.image_y, 60, 104);
			door_blue.Draw(memdc, blue_door.x, blue_door.y, 60, 100, blue_door.image_x, blue_door.image_y, 60, 104);
			//button_img.Draw(memdc, button[0].x, button[0].y - button[0].image_y, 40, button[0].image_y, 0, 0, button[0].image_x, button[0].image_y);

			if (fire.on)
			{
				if (fire.Down == TRUE && fire.dic == 0) {
					Fire[3].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0, 0, 215, 411);
				}
				else if (fire.dic == 0) {
					Fire[0].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0, 0, 215, 411);
				}
				else if (fire.dic == -1) {
					Fire[4].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0 + 342 * F_Frame, 0, 342, 271);
				}
				else if (fire.dic == 1) {
					Fire[2].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0 + 342 * F_Frame, 0, 342, 271);
				}
			}

			if (water.on)
			{
				if (water.Down == TRUE && water.dic == 0) {
					Water[3].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 140, 0, 0, 203, 553);
				}
				else if (water.dic == 0) {
					Water[0].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0, 0, 207, 400);
				}
				else if (water.dic == -1) {
					Water[4].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0 + 376 * W_Frame, 0, 376, 400);
				}
				else if (water.dic == 1) {
					Water[2].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0 + 376 * W_Frame, 0, 376, 400);
				}
			}

			clock.Draw(memdc, 525, 0, 150, 50, 0, 0, 154, 54);
			switch (time / 60) {
			case 0:
				zero.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 1:
				one.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 2:
				two.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 3:
				three.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 4:
				four.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 5:
				five.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 6:
				six.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 7:
				seven.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 8:
				eight.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 9:
				nine.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
				break;
			}
			switch ((time % 60) / 10) {
			case 0:
				zero.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 1:
				one.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 2:
				two.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 3:
				three.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 4:
				four.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 5:
				five.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 6:
				six.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 7:
				seven.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 8:
				eight.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 9:
				nine.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
				break;
			}
			switch ((time % 60) % 10) {
			case 0:
				zero.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 1:
				one.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 2:
				two.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 3:
				three.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 4:
				four.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 5:
				five.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 6:
				six.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 7:
				seven.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 8:
				eight.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			case 9:
				nine.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
				break;
			}
		}
		else if (stage == 3)
		{
		stage1.Draw(memdc, 0, 0, 1190, 765, 0, 480 - average, 640, 480);

		for (int i = 0; i < 20 && Ft[i].x != NULL; ++i)
		{
			foot_block.Draw(memdc, Ft[i].x, Ft[i].y, Ft[i].wid, Ft[i].hei, 0, 0, 111, 23);
		}

		rect.Draw(memdc, Rt.x-50, Rt.y-50, 50, 50, 0, 0, 40, 40);


		block_w.Draw(memdc, block[0].x+block[0].image_x, block[0].y, 100+block[0].image_x, 30, -block[0].image_x, 0, 83+block[0].image_x, 25);

		door_red.Draw(memdc, red_door.x, red_door.y, 60, 100, red_door.image_x, red_door.image_y, 60, 104);
		door_blue.Draw(memdc, blue_door.x, blue_door.y, 60, 100, blue_door.image_x, blue_door.image_y, 60, 104);

		button_img.Draw(memdc, button[0].x, button[0].y - button[0].image_y, 40, button[0].image_y, 0, 0, button[0].image_x, button[0].image_y);

		if (fire.on)
		{
			if (fire.Down == TRUE && fire.dic == 0) {
				Fire[3].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0, 0, 215, 411);
			}
			else if (fire.dic == 0) {
				Fire[0].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0, 0, 215, 411);
			}
			else if (fire.dic == -1) {
				Fire[4].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0 + 342 * F_Frame, 0, 342, 271);
			}
			else if (fire.dic == 1) {
				Fire[2].Draw(memdc, fire.x - 60, fire.y - 100 + average, 60, 100, 0 + 342 * F_Frame, 0, 342, 271);
			}
		}

		if (water.on)
		{
			if (water.Down == TRUE && water.dic == 0) {
				Water[3].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 140, 0, 0, 203, 553);
			}
			else if (water.dic == 0) {
				Water[0].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0, 0, 207, 400);
			}
			else if (water.dic == -1) {
				Water[4].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0 + 376 * W_Frame, 0, 376, 400);
			}
			else if (water.dic == 1) {
				Water[2].Draw(memdc, water.x - 60, water.y - 100 + average, 60, 100, 0 + 376 * W_Frame, 0, 376, 400);
			}
		}

		clock.Draw(memdc, 525, 0, 150, 50, 0, 0, 154, 54);
		switch (time / 60) {
		case 0:
			zero.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 1:
			one.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 2:
			two.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 3:
			three.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 4:
			four.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 5:
			five.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 6:
			six.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 7:
			seven.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 8:
			eight.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 9:
			nine.Draw(memdc, 530, 0, 50, 50, 0, 0, 50, 50);
			break;
		}
		switch ((time % 60) / 10) {
		case 0:
			zero.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 1:
			one.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 2:
			two.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 3:
			three.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 4:
			four.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 5:
			five.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 6:
			six.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 7:
			seven.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 8:
			eight.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 9:
			nine.Draw(memdc, 585, 0, 50, 50, 0, 0, 50, 50);
			break;
		}
		switch ((time % 60) % 10) {
		case 0:
			zero.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 1:
			one.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 2:
			two.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 3:
			three.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 4:
			four.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 5:
			five.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 6:
			six.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 7:
			seven.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 8:
			eight.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		case 9:
			nine.Draw(memdc, 620, 0, 50, 50, 0, 0, 50, 50);
			break;
		}
		}
		if (Die.On)
		{
			die.Draw(memdc, Die.x, Die.y, 100, 100, Die.image_x, Die.image_y, 159, 89);
		}

		if (clear)
		{
			back = TRUE;
		}
		if (time_over)
		{
			timeout.Draw(memdc, 400, 200, 400, 250, 0, 0, 486, 286);
		}
		if (stair)
		{
			red_stair.Draw(memdc, red_door.x, red_door.y+30, 50, 80, stair_red_x, 0, 50, 73);
			blue_stair.Draw(memdc, blue_door.x , red_door.y+30 , 50, 80, stair_blue_x, 0, 54, 77);
			fire.on = FALSE;
			water.on = FALSE;
		}
		//switch()
		if (back)
		{
			backimg.Draw(memdc, 0, 0, 1200, 800, 0, 0, 1200, 800);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			KillTimer(hWnd, 5);
			clear = FALSE;
			mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
		}
		BitBlt(hDC, 0, 0, 1200, 800, memdc, 0, 0, SRCCOPY);
		if (clear == FALSE && back==FALSE )
		{
			for (int i = 0; i < 20; i++)
			{
				if (Jewelry[i].On)
				{
					if (i < 10)
					{
						Jewelry_blue.Draw(hDC, Jewelry[i].x, Jewelry[i].y, 28, 25, Jewelry[i].image_x, Jewelry[i].image_y, 28, 24);
					}

					if (i < 20 && i >= 10)
					{
						Jewelry_red.Draw(hDC, Jewelry[i].x, Jewelry[i].y, 28, 25, Jewelry[i].image_x, Jewelry[i].image_y, 29, 24);
					}
				}
			}

			for (int i = 0; i < 90; i++) // 0~19까지 파랑 가운데 물 20~29까지 파랑 왼.오 30~49까지 빨강 가운데 물 50~59까지 빨강물 왼.오 60~79 초록 가운데 80~89 초록왼.오
			{
				if (Trap[i].On)
				{
					if (i < 20)
					{
						blue_water_center.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 23, 14);
					}
					if (i >= 20 && i < 25)
					{
						blue_water_left.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 23, 23);
					}
					if (i >= 25 && i < 30)
					{
						blue_water_right.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 23, 23);
					}
					if (i >= 30 && i < 50)
					{
						red_water_center.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 21, 14);
					}
					if (i >= 50 && i < 55)
					{
						red_water_left.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 24, 25);
					}
					if (i >= 55 && i < 60)
					{
						red_water_right.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 24, 25);
					}
					if (i >= 60 && i < 80)
					{
						green_water_center.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 23, 14);
					}
					if (i >= 80 && i < 85)
					{
						green_water_left.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 24, 25);
					}
					if (i >= 85 && i < 90)
					{
						green_water_right.Draw(hDC, Trap[i].x, Trap[i].y, 25, 25, Trap[i].image_x, Trap[i].image_y, 24, 25);
					}
				}
			}

		}
		DeleteDC(hDC);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave)
{
	DWORD Result;
	mciOpenParms.lpstrDeviceType = L"MPEGvideo";
	mciOpenParms.lpstrElementName = lpszWave;
	Result = mciSendCommand(wDeviceID, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms);
	if (Result)
		return Result;
	wDeviceID = mciOpenParms.wDeviceID;
	mciPlayParms.dwCallback = (DWORD)hWnd;
	if (Result)
		return Result;
	return 0;
}

void LoadSound(HWND hWnd)
{
	DWORD SelectBGM;

	//if (stage == 0)
	//{
	//	mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
	//	SelectBGM = LoadWAV(hWnd, L"BGM\\브금1.mp3");//여기에 재생하고싶은 mp3파일의 경로를 입력하면 된다.
	//	SelectBGM = mciSendCommand(1, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms);
	//}
	//else if (stage == 1)
	//{
	//	mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
	//	SelectBGM = LoadWAV(hWnd, L"BGM\\브금2.mp3");//여기에 재생하고싶은 mp3파일의 경로를 입력하면 된다.
	//	SelectBGM = mciSendCommand(1, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms);
	//}
	//else if (stage == 2)
	//{
	//	mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
	//	SelectBGM = LoadWAV(hWnd, L"BGM\\브금3.mp3");//여기에 재생하고싶은 mp3파일의 경로를 입력하면 된다.
	//	SelectBGM = mciSendCommand(1, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms);
	//}
	//else if (stage == 3)
	//{
	//	mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
	//	SelectBGM = LoadWAV(hWnd, L"BGM\\브금4.mp3");//여기에 재생하고싶은 mp3파일의 경로를 입력하면 된다.
	//	SelectBGM = mciSendCommand(1, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms);
	//}
	//else if (stage == 4)
	//{
	//	mciSendCommand(1, MCI_CLOSE, 0, (DWORD)NULL);
	//	SelectBGM = LoadWAV(hWnd, L"BGM\\브금5.mp3");//여기에 재생하고싶은 mp3파일의 경로를 입력하면 된다.
	//	SelectBGM = mciSendCommand(1, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms);
	//}
}

void Jump() {
	if (fire.Down) {
		if (fire.y + (fire.v + fire.g) > fire.ground) {
			fire.is_Jumping = FALSE;
			fire.Down = FALSE;
			fire.v = 50;
			fire.y = fire.ground;
		}
		if (fire.y < fire.ground) {
			fire.v = fire.v + fire.g;
			fire.y = fire.y + fire.v;
		}
	}
	if (water.Down) {
		if (water.y + (water.v + water.g) > water.ground) {
			water.is_Jumping = FALSE;
			water.Down = FALSE;
			water.v = 50;
			water.y = water.ground;
		}
		if (water.y < water.ground) {
			water.v = water.v + water.g;
			water.y = water.y + water.v;
		}
	}
	if (water.is_Jumping == TRUE) {
		if (!water.Down) {
			if (water.v >= 0) {
				water.v = water.v - water.g;
				water.y = water.y - (water.v / 2);
			}
			else {
				water.Down = TRUE;    // 상태 변화
			}
		}
	}
	if (fire.is_Jumping == TRUE) {
		if (!fire.Down) {
			if (fire.v > 0) {
				fire.v = fire.v - fire.g;
				fire.y = fire.y - (fire.v / 2);
			}
			else {
				fire.Down = TRUE;    // 상태 변화
			}
		}
	}
}

void Wid_Move() {
	if (water.is_Move) {
		if (water.dic == 1) {
			if (water.wid_v <= 10) {
				water.wid_v = water.wid_v + water.wid_a;
			}
			water.x = water.x + water.wid_v;
		}
		else if (water.dic == -1) {
			if (water.wid_v <= 10) {
				water.wid_v = water.wid_v + water.wid_a;
			}
			water.x = water.x - water.wid_v;
		}
	}
	if (water.is_Speed_Down) {
		if (water.dic == 1) {
			water.wid_a += 1;
			water.wid_v = water.wid_v - water.wid_a;
			water.x = water.x + water.wid_v;
			if (water.wid_v <= 0) {
				water.wid_v = 0;
				water.wid_a = 0;
				water.dic = 0;
				water.is_Speed_Down = FALSE;
			}
		}
		else if (water.dic == -1) {
			water.wid_a += 1;
			water.wid_v = water.wid_v - water.wid_a;
			water.x = water.x - water.wid_v;
			if (water.wid_v <= 0) {
				water.wid_v = 0;
				water.wid_a = 0;
				water.dic = 0;
				water.is_Speed_Down = FALSE;
			}
		}
	}
	if (fire.is_Move) {
		if (fire.dic == 1) {
			if (fire.wid_v <= 10) {
				fire.wid_v = fire.wid_v + fire.wid_a;
			}
			fire.x = fire.x + fire.wid_v;
		}
		else if (fire.dic == -1) {
			if (fire.wid_v <= 10) {
				fire.wid_v = fire.wid_v + fire.wid_a;
			}
			fire.x = fire.x - fire.wid_v;
		}
	}
	if (fire.is_Speed_Down) {
		if (fire.dic == 1) {
			fire.wid_a += 1;
			fire.wid_v = fire.wid_v - fire.wid_a;
			fire.x = fire.x + fire.wid_v;
			if (fire.wid_v <= 0) {
				fire.wid_v = 0;
				fire.wid_a = 0;
				fire.dic = 0;
				fire.is_Speed_Down = FALSE;
			}
		}
		else if (fire.dic == -1) {
			fire.wid_a += 1;
			fire.wid_v = fire.wid_v - fire.wid_a;
			fire.x = fire.x - fire.wid_v;
			if (fire.wid_v <= 0) {
				fire.wid_v = 0;
				fire.wid_a = 0;
				fire.dic = 0;
				fire.is_Speed_Down = FALSE;
			}
		}
	}
}

void Push() {
	if (water.is_Push == FALSE && fire.is_Push == FALSE) {
		if (water.y == Rt.y && abs(water.x - Rt.x) <= 60) {			// 불과 접촉
			water.is_Push = TRUE;
		}
		if (fire.y == Rt.y && abs(fire.x - Rt.x) <=60) {		// 물과 접촉
			fire.is_Push = TRUE;
		}
	}
	// 충돌 체크
	if (water.is_Push == TRUE && water.y == Rt.y && abs(water.x - Rt.x) <= 60) {
		if (Rt.dic == 0) {
			Rt.dic = water.dic;
		}

		if (Rt.dic == water.dic) {
			if (Rt.dic == 1) {
				Rt.x = water.x + 60;
			}
			else if (Rt.dic == -1) {
				Rt.x = water.x - 60;
			}
		}
	}
	else if (water.is_Push == TRUE && Rt.dic != water.dic) {
		Rt.dic = 0;
		water.is_Push = FALSE;
		return;
	}

	if (fire.is_Push == TRUE && fire.y == Rt.y && abs(fire.x - Rt.x) <= 60) {
		if (Rt.dic == 0) {
			Rt.dic = fire.dic;
		}

		if (Rt.dic == fire.dic) {
			if (Rt.dic == 1) {
				Rt.x = fire.x + 60;
			}
			else if (Rt.dic == -1) {
				Rt.x = fire.x - 60;
			}
		}
	}
	else if (fire.is_Push == TRUE && Rt.dic != fire.dic) {
		Rt.dic = 0;
		fire.is_Push = FALSE;
		return;
	}

	for (int i = 0; i < 20 && Ft[i].x != NULL; ++i) {
		if ((Rt.y == Ft[i].y && Ft[i].x > Rt.x - 60) || (Rt.y == Ft[i].y && Ft[i].x + Ft[i].wid < Rt.x)) {
			Rt.Down = TRUE;
		}
	}
	if (Rt.Down) {
		if (Rt.y + (Rt.v + Rt.g) >= 730) {
			Rt.v = 0;
			Rt.y = 730;
			Rt.Down = FALSE;
			return;
		}
		if (Rt.y <=730) {
			Rt.v = Rt.v + Rt.g;
			Rt.y = Rt.y + Rt.v;
			return;
		}
	}
}

void Climb() {
	double a = (hill.x + (hill.x - hill.wid)) / 2;
	double b = (hill.y + (hill.y + hill.hei)) / 2;

	if (hill.dic == 1) {
		double A = (-1) * (hill.hei / hill.wid);
		double B = b - A * a;

		if (hill.x - hill.wid < fire.x && fire.x - 20 <= hill.x) {
			if (fire.is_Speed_Down == TRUE) {
				if (fire.y < fire.ground) {

					fire.wid_a += 1;
					if (water.wid_v <= 10) {
						water.wid_v = water.wid_v + water.wid_a;
					}
					water.x = water.x - water.wid_v;
					fire.y = (int)(A * fire.x + B) + 10;
					return;
				}
				else {
					fire.y = fire.ground;
					fire.is_Speed_Down = FALSE;
				}
			}
			else {
				fire.y = (int)(A * fire.x + B) + 10;
			}
		}
		if (hill.x - hill.wid < water.x && water.x - 20 <= hill.x) {
			water.y = (int)(A * water.x + B) + 10;
			water.ground = (int)(A * water.x + B) + 10;
		}
	}
	else if (hill.dic == -1) {
		double A = (hill.hei / hill.wid);
		double B = b - A * a;

	}
}

void Foot() {
	for (int i = 0; i < 20; ++i) {
		if (Ft[i].W_On) {
			if (Ft[i].x> water.x || (water.x - 60) > (Ft[i].x + Ft[i].wid)) {    // 밖으로 나갔을 경우
				bool signal = FALSE;
				for (int beam = water.y; beam < 730; ++beam) {            // 빔 쏘기
					for (int j = 0; j < 20; ++j) {    // 블럭 하나씩 검사
						if (((Ft[j].y + average) - beam) < 10 && ((Ft[j].y + average) - beam) > -10) {
							if (Ft[j].x < water.x - 30 && water.x - 30 < Ft[j].x + Ft[j].wid) {            // if (Ft[i].x <= water.x && water.x - 60 <= Ft[i].x + Ft[i].wid)
								water.ground = beam;
								Ft[j].W_On = FALSE;
								signal = TRUE;
							}
						}
					}
				}
				if (!signal) {
					water.ground = 730;
				}
				if (water.is_Jumping == FALSE) {
					water.v = 0;
					water.Down = TRUE;
				}
				Ft[i].W_On = FALSE;
			}
		}
		if (Ft[i].F_On) {
			if (Ft[i].x > fire.x || fire.x - 60 > Ft[i].x + Ft[i].wid) {    // 밖으로 나갔을 경우
				bool signal = FALSE;
				for (int beam = fire.y; beam < 730; ++beam) {            // 빔 쏘기
					for (int j = 0; j < 20; ++j) {    // 블럭 하나씩 검사
						if ((Ft[j].y + average) - beam < 10 && (Ft[j].y + average) - beam > -10) {
							if (Ft[j].x < fire.x && fire.x - 60 < Ft[j].x + Ft[j].wid) {
								fire.ground = beam;
								Ft[j].W_On = FALSE;
								signal = TRUE;
							}
						}
					}
				}
				if (!signal) {
					fire.ground = 730;
				}
				if (fire.is_Jumping == FALSE) {
					fire.v = 0;
					fire.Down = TRUE;
				}
				Ft[i].F_On = FALSE;
			}
		}
		if (water.is_Jumping == TRUE) {
			if (Ft[i].x <= water.x && water.x - 60 <= Ft[i].x + Ft[i].wid) {
				if ((water.y - 70) - (Ft[i].y + Ft[i].hei + average) <= 5 && (water.y - 70) - (Ft[i].y + Ft[i].hei + average) > Ft[i].hei * -1) {    // 머리 부닥치기
					water.v = 0;
					water.Down = TRUE;
				}
				if (Ft[i].y + average - water.y <= 5 && Ft[i].y + average - water.y > -20) {        // 안착
					water.is_Jumping = FALSE;
					water.Down = FALSE;
					water.v = 50;
					water.y = Ft[i].y + average;
					water.ground = Ft[i].y + average;
					Ft[i].W_On = TRUE;
				}
			}
			if (Ft[i].y + average > water.y - 60 && Ft[i].y + Ft[i].hei + average < water.y) {
				if (Ft[i].x - 5 - water.x < 20 && Ft[i].x - 5 - water.x > 0) {
					water.wid_a = 0;
					water.wid_v = 0;
				}
				if ((water.x - 60) - (Ft[i].x + Ft[i].wid + 5) < 20 && (water.x - 60) - (Ft[i].x + Ft[i].wid + 5) > 0) {
					water.wid_a = 0;
					water.wid_v = 0;
				}
			}
		}
		if (fire.is_Jumping == TRUE) {
			if (Ft[i].x< fire.x && fire.x - 60 < Ft[i].x + Ft[i].wid) {
				if ((fire.y - 70) - (Ft[i].y + Ft[i].hei + average) <= 5 && (fire.y - 70) - (Ft[i].y + Ft[i].hei + average) > Ft[i].hei * -1) {
					fire.v = 0;
					fire.Down = TRUE;
				}
				if (Ft[i].y + average - fire.y <= 5 && Ft[i].y + average - fire.y > -20) {
					fire.is_Jumping = FALSE;
					fire.Down = FALSE;
					fire.v = 50;
					fire.y = Ft[i].y + average;
					fire.ground = Ft[i].y + average;
					Ft[i].F_On = TRUE;
				}
			}
			if (Ft[i].y + average > fire.y - 70 && Ft[i].y + Ft[i].hei + average < fire.y) {
				if (Ft[i].x - 5 - fire.x < 20 && Ft[i].x - 5 - fire.x > 0) {
					fire.wid_a = 0;
					fire.wid_v = 0;
				}
				if ((fire.x - 60) - (Ft[i].x + Ft[i].wid + 5) < 20 && (fire.x - 60) - (Ft[i].x + Ft[i].wid + 5) > 0) {
					fire.wid_a = 0;
					fire.wid_v = 0;
				}
			}
		}
	}
}