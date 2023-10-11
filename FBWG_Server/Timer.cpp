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
	startTime = std::chrono::steady_clock::now();
}

long long Timer::GetElapsedTime()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(current - startTime).count();
}