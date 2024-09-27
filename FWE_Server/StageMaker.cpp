#include "StageMaker.h"

StageMaker::StageMaker()
	: stage_index(STAGE_TYPE::STAGE_TITLE)
{
}

void StageMaker::run_game_stage_thread(array<Client, 3>* game_member, Stage* game_stage) {
	clients = game_member;
	stage_item = game_stage;
	
	stage_index = STAGE_TYPE::STAGE_ROLE;
	while (stage_index == STAGE_TYPE::STAGE_ROLE) {
		check_all_client_role();
	}
	
	while (true) {
		/*if (check_next_stage_condition()) {
			show_game_stage(stage_index);

			if (stage_index == STAGE_TYPE::STAGE_TITLE) {
				break;
			}
			reset_game_stage();
		}*/

		check_jewely();
		check_door();

	}

	cleanup_game();
}

bool StageMaker::check_next_stage_condition() {
	//select_mutex.lock();
	for (Client& client : *clients) {
		if (stage_index != client.get_curr_stage()) {
			//select_mutex.unlock();
			return true;
		}
	}
	//select_mutex.unlock();
	return false;
}

void StageMaker::check_all_client_role() {
	for (Client& client : *clients) {
		if (false == client.role) {
			return;
		}
	}

	stage_index = STAGE_TYPE::STAGE_01;
	if (check_next_stage_condition()) {
		show_game_stage(stage_index);
	}
}

void StageMaker::show_game_stage(int stage_number) {
	reset_game_stage();

	StageUpdatePacket stage_update(clients); 
	stage_update.sync_send_packet(&stage_index);

	(*stage_item).reset_position(stage_index, clients);
	
	ClientMovePacket move(clients);
	move.sync_send_packet(NULL);
}

void StageMaker::reset_game_stage() {
	_timer.Reset();
	_timer.start_timer();
}

bool StageMaker::show_player_score() {
	for (Client& client : *clients) {
		if ((*stage_item).jewely.front().OBJECT_Collide(client)) {
			client.score += 1;

			S2CPlayerPacket packet;
			packet.type = static_cast<int>(PACKET_TYPE_S2C::EatJewely);
			packet.id = client.user_ticket;
			for (Client& sync_client : *clients) {
				send(sync_client.network_socket, reinterpret_cast<char*>(&packet), sizeof(S2CPlayerPacket), 0);
			}
			return true;
		}
	}
	return false;
}

bool StageMaker::check_jewely() {
	if (false == (*stage_item).jewely.empty()) {
		if (show_player_score()) {
			currentJewelyNum++;
			(*stage_item).jewely.pop();
			// (*stage_item).currentVisibleJewely = (*stage_item).jewely.front();
		}
	}

	return true;
}

bool StageMaker::check_door() {
	if (!isVisibleDoor
		&& (currentJewelyNum == (*stage_item).maxJewelyNum)
		&& (!isVisibleDoor && currentJewelyNum == (*stage_item).maxJewelyNum)) {
		StageDoorOpenSyncPacket door_open(clients);
		door_open.sync_send_packet(NULL);
		isVisibleDoor = true;
		return true;
	}
	return false;

	// each player check into door
	if (isVisibleDoor) {
		for (Client& client : *clients) {
			if ((*stage_item).door.OBJECT_Collide(client)) {
				S2CPlayerPacket intoDoorPacket;
				intoDoorPacket.id = client.user_ticket;
				intoDoorPacket.type = static_cast<int>(PACKET_TYPE_S2C::IntoDoor);

				for (Client& send_client : *clients) {
					send(send_client.network_socket, reinterpret_cast<char*>(&intoDoorPacket), sizeof(S2CPlayerPacket), 0);
				}
			}
		}
	}

	// check next stage 
	if (stage_index != STAGE_TYPE::RESULT) {
		stage_index += 1;
	}
	else {
		stage_index = STAGE_TYPE::STAGE_TITLE;
	}
}

void StageMaker::cleanup_game() {
	for (Client& client : *clients) {
		closesocket(client.network_socket);
	}
	WSACleanup();
}
