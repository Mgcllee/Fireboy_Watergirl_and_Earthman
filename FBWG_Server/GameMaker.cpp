#include "GameMaker.h"

GameMaker::GameMaker()
	: stage_maker_handle(NULL)
{
	for (HANDLE& handle : client_handle) {
		handle = NULL;
	}
}

GameMaker::~GameMaker()
{
}

void GameMaker::run_game() {
	while (true) {
		create_game_threads();
		reset_game();
		start_game();
		join_game_threads();
	}
}
void GameMaker::create_game_threads() {
	stage_maker_thread = thread(&StageMaker::run_game_stage_thread, new StageMaker);
	
	for (thread& client_thread : client_threads) {
		client_thread = thread(&Client::run_client_thread, new Client());
	}
}
void GameMaker::join_game_threads() {
	for (thread& client_thread : client_threads) {
		client_thread.join();
	}
	stage_maker_thread.join();
}


void GameMaker::reset_game()
{

}

void GameMaker::start_game()
{

}