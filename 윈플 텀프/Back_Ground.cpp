#pragma once
#include "stdafx.h"
#include "protocol.h"
#include "Stage.h"

void Move()
{
	/*if (players[currneClientNum].wid_a <= 10.f)
		players[currneClientNum].wid_a += 0.1f;
	if (players[currneClientNum].wid_v <= 10.f)
		players[currneClientNum].wid_v += players[currneClientNum].wid_a;*/

	MovePacket move;

	if (keybuffer[VK_LEFT]) {
		move.x = -1;
		//players[currneClientNum].direction = -1;
		//players[currneClientNum].x -= players[currneClientNum].wid_v;
		//for (OBJECT& ft : currentStage.Ft) {
		//	if (ft.Ft_Collision(players[currneClientNum])) {
		//		players[currneClientNum].x += players[currneClientNum].wid_v;
		//		break;
		//	}
		//}
	}
	if (keybuffer[VK_RIGHT]) {
		move.x = 1;
		//players[currneClientNum].direction = 1;
		//players[currneClientNum].x += players[currneClientNum].wid_v;
		//for (OBJECT& ft : currentStage.Ft) {
		//	if (ft.Ft_Collision(players[currneClientNum])) {
		//		players[currneClientNum].x -= players[currneClientNum].wid_v;
		//		break;
		//	}
		//}
	}
	if (keybuffer[VK_UP]) {
		move.y = SHRT_MAX;
	}

	if (keybuffer[VK_LEFT] || keybuffer[VK_RIGHT] || keybuffer[VK_UP]) {
		move.type = C2SMove;
		move.id = myId;
		SendPacket(&move);
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

