#include "Timer.h"

Timer::Timer()
{
	isRunning = false;
}
Timer::~Timer()
{
	if (isRunning == true)
	{
		Stop();
		woker.join();
	}
}
bool Timer::IsRunning()
{
	return isRunning;
}
void Timer::Start(const Milliseconds& milliseconds, const IntervalCallback& intervalCallback)
{
	if (isRunning == true)
	{
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
void Timer::Stop()
{
	isRunning = false;
	//woker.join();
}

void Timer::Reset()
{
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
	if (_timer.IsRunning() == true)
	{
		return;
	}

	_timer.Start(std::chrono::milliseconds(1000), [=]
		{
			S2CStageTimePassPacket packet;
			packet.timePassed = _timer.GetElapsedTime() / (double)1000;

			for (int x = 0; x < 3; x++) {
				send(threadHandles[x].clientSocket, (char*)&packet, sizeof(S2CStageTimePassPacket), 0);
			}

			if (timeoutSeconds <= packet.timePassed && !isTimeOut && !gameEnd)
			{
				typePacket timeoutPacket;
				for (int x = 0; x < 3; x++) {
					send(threadHandles[x].clientSocket, (char*)&timeoutPacket, sizeof(typePacket), 0);
				}
				DWORD retValDoor0 = WaitForSingleObject(threadHandles[0].intDoor, 0);
				DWORD retValDoor1 = WaitForSingleObject(threadHandles[1].intDoor, 0);
				DWORD retValDoor2 = WaitForSingleObject(threadHandles[2].intDoor, 0);

				if (retValDoor0 != WAIT_OBJECT_0 && retValDoor1 != WAIT_OBJECT_0 && retValDoor2 != WAIT_OBJECT_0) {
					S2CChangeStagePacket changePacket;
					changePacket.stageNum = RESULT;
					changePacket.type = S2CChangeStage;
					for (int x = 0; x < 3; x++) {
						send(threadHandles[x].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
					}
					isTimeOut = false;
					gameEnd = true;
					TimeoutStage();
				}
				else {
					if (retValDoor0 != WAIT_OBJECT_0 && threadHandles[0].isArrive) {
						S2CPlayerPacket playerPacket;
						playerPacket.id = threadHandles[0].clientId;
						cout << "out Player: " << (int)playerPacket.id << endl;
						playerPacket.type = S2CPlayerOut;
						threadHandles[0].isArrive = false;
						for (int x = 0; x < 3; x++) {
							send(threadHandles[x].clientSocket, (char*)&playerPacket, sizeof(S2CPlayerPacket), 0);
						}
					}
					if (retValDoor1 != WAIT_OBJECT_0 && threadHandles[1].isArrive) {
						S2CPlayerPacket playerPacket;
						playerPacket.id = threadHandles[1].clientId;
						cout << "out Player: " << (int)playerPacket.id << endl;
						playerPacket.type = S2CPlayerOut;
						threadHandles[1].isArrive = false;
						for (int x = 0; x < 3; x++) {
							send(threadHandles[x].clientSocket, (char*)&playerPacket, sizeof(S2CPlayerPacket), 0);
						}
					}
					if (retValDoor2 != WAIT_OBJECT_0 && threadHandles[2].isArrive) {
						S2CPlayerPacket playerPacket;
						playerPacket.id = threadHandles[2].clientId;
						cout << "out Player: " << (int)playerPacket.id << endl;
						playerPacket.type = S2CPlayerOut;
						threadHandles[2].isArrive = false;
						for (int x = 0; x < 3; x++) {
							send(threadHandles[x].clientSocket, (char*)&playerPacket, sizeof(S2CPlayerPacket), 0);
						}
					}
					isTimeOut = true;
				}

			}
			if (packet.timePassed >= 35 && !isTimeOut && !gameEnd) {
				if (!isVisibleDoor) {
					typePacket visibleDoorPacket;
					visibleDoorPacket.type = S2CDoorVisible;
					for (int x = 0; x < 3; x++) {
						send(threadHandles[x].clientSocket, (char*)&visibleDoorPacket, sizeof(typePacket), 0);
					}
					isVisibleDoor = true;
				}
			}
		});
}