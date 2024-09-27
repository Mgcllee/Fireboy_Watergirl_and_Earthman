#include "Timer.h"

Timer::Timer(array<Client, 3>* member)
	: isRunning(false)
	, clients(member)
{
}

Timer::~Timer() {
	if (isRunning == true) {
		Stop();
		woker.join();
	}
}

bool Timer::IsRunning() {
	return isRunning;
}

void Timer::Start(const Milliseconds& milliseconds, const IntervalCallback& intervalCallback) {
	if (isRunning == true) {
		return;
	}

	startTime = std::chrono::steady_clock::now();
	isRunning = true;
	woker = std::thread([=]
		{
			while (isRunning == true)
			{
				std::this_thread::sleep_for(milliseconds); 
				intervalCallback();
			}
		});
}

void Timer::Stop() {
	isRunning = false;
}

void Timer::Reset() {
	time_out = false;
	startTime = std::chrono::steady_clock::now();
}

long long Timer::GetElapsedTime()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(current - startTime).count();
}

void Timer::start_timer()
{
	if (IsRunning() == true) {
		return;
	}

	Start(std::chrono::milliseconds(1000), [=]
		{
			S2CStageTimePassPacket packet;
			packet.type = static_cast<char>(PACKET_TYPE_S2C::StageTimePass);
			packet.timePassed = static_cast<int>(GetElapsedTime() / 1000);

			for (Client& client : *clients) {
				send(client.network_socket, reinterpret_cast<char*>(&packet), sizeof(packet), 0);
			}

			if (timeoutSeconds <= packet.timePassed && !isTimeOut && !gameEnd)
			{
				typePacket timeoutPacket;
				timeoutPacket.type = static_cast<char>(PACKET_TYPE_S2C::StageTimeout);
				for (Client& client : *clients) {
					send(client.network_socket, reinterpret_cast<char*>(&timeoutPacket), sizeof(timeoutPacket), 0);
				}


				int wait_counter_result_stage = 0;
				for (Client& client : *clients) {
					if (STAGE_TYPE::RESULT == client.curr_stage_type) {
						wait_counter_result_stage += 1;
					}
				}

				if (3 == wait_counter_result_stage) {
					S2CChangeStagePacket changePacket;
					changePacket.stageNum = STAGE_TYPE::RESULT;
					changePacket.type = static_cast<char>(PACKET_TYPE_S2C::ChangeStage);
					for (Client& client : *clients) {
						send(client.network_socket, reinterpret_cast<char*>(&changePacket), sizeof(changePacket), 0);
					}

					isTimeOut = false;
					gameEnd = true;
				}
				else {
					S2CPlayerPacket playerPacket;
					playerPacket.type = static_cast<char>(PACKET_TYPE_S2C::PlayerOut);
					playerPacket.size = sizeof(S2CPlayerPacket);
					for (Client& client : *clients) {
						if (client.isArrive) {
							playerPacket.id = client.user_ticket;
							client.isArrive = false;
							for (Client& send_client : *clients) {
								send(send_client.network_socket, reinterpret_cast<char*>(&playerPacket), sizeof(playerPacket), 0);
							}
						}
					}

					isTimeOut = true;
				}
			}
			if (packet.timePassed >= 35 && !isTimeOut && !gameEnd) {
				/*if (!isVisibleDoor) {
					typePacket visibleDoorPacket;
					visibleDoorPacket.type = static_cast<char>(PACKET_TYPE_S2C::DoorVisible);
					for (Client& send_client : *clients) {
						send(send_client.network_socket, reinterpret_cast<char*>(&visibleDoorPacket), sizeof(visibleDoorPacket), 0);
					}
					isVisibleDoor = true;
				}*/
			}
		});
}