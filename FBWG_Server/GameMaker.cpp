#include "GameMaker.h"

GameMaker::GameMaker()
{
}

GameMaker::~GameMaker()
{
}

void GameMaker::run_game()
{
	while (true) {
		make_game_stage();

		HANDLE serverThread = CreateThread(NULL, 0, ServerWorkThread, reinterpret_cast<LPVOID>(1), 0, NULL);

		while (WSA_WAIT_EVENT_0 + 2
			!= WSAWaitForMultipleEvents(
				3, multiEvenTthreadHadle, TRUE, WSA_INFINITE, FALSE)
			) {

		}

		for (int j = 0; j < 3; j++) {
			CloseHandle(threadHandles[j].threadHandle);
		}
		CloseHandle(serverThread);
	}
}

void GameMaker::cleanup_game()
{
}
