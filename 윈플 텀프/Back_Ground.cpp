#pragma once
#include"stdafx.h"

// 윈도우 프로그래밍에서 키 동시 입력을 위한 함수 (제거 예정)
void Loop(bool keyDown)
{
	if (true == keyDown) {
		//다중키 입력을 위해서 전부 if로 작성했으며 [ ] 안에 원하는 키와 행동을 넣으면 작동한다.
		if (keybuffer[VK_LEFT])
		{
			fire.wid_a += 1;
			if (fire.wid_a > 5) {
				fire.wid_a = 10;
			}
			fire.dic = -1;
			fire.is_Move = TRUE;
		}
		if (keybuffer[VK_RIGHT])
		{
			fire.wid_a += 1;
			if (fire.wid_a > 5) {
				fire.wid_a = 10;
			}
			fire.dic = 1;
			fire.is_Move = TRUE;
		}
		if (keybuffer[VK_UP])
		{
			fire.is_Jumping = TRUE;
		}
		if (keybuffer[VK_DOWN])
		{
			//	button[0].On = FALSE;
			//	block[0].On = TRUE;
		}
		if (keybuffer['d'] || keybuffer['D'])
		{
			water.wid_a += 1;
			if (water.wid_a > 5) {
				water.wid_a = 10;
			}
			water.dic = 1;
			water.is_Move = TRUE;
		}
		if (keybuffer['a'] || keybuffer['A'])
		{
			water.wid_a += 1;
			if (water.wid_a > 5) {
				water.wid_a = 10;
			}
			water.dic = -1;
			water.is_Move = TRUE;
		}
		if (keybuffer['s'] || keybuffer['S'])
		{
			//	button[0].On = TRUE;
		}
		if (keybuffer['w'] || keybuffer['W'])
		{
			water.is_Jumping = TRUE;
		}
	}
	else if (false == keyDown) {
		water.is_Move = FALSE;
		water.wid_a = 0;
		water.is_Speed_Down = TRUE;
		fire.is_Move = FALSE;
		fire.wid_a = 0;
		water.Frame = 0;
		fire.Frame = 0;
		fire.is_Speed_Down = TRUE;
	}
}

// WAV형식 음원 분석 함수 (LoadSound()함수에서만 호출)
DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave)
{
	MCI_OPEN_PARMS	mciOpenParms;
	UINT			wDeviceID = 0;

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

// WAV형식 음원 스테이지에 맞게 출력하기 (Main.cpp에서 스테이지 변경시 호출 필요)
void LoadSound(HWND hWnd)
{
	DWORD SelectBGM{};

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

