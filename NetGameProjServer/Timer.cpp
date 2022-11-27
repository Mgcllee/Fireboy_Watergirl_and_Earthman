#include "Timer.h"

Timer::Timer()
{
	_isRunning = false;
}
Timer::~Timer()
{
	if (_isRunning == true)
	{
		Stop();
	}
}
bool Timer::IsRunning()
{
	return _isRunning;
}
void Timer::Start(const Milliseconds& milliseconds, const IntervalCallback& intervalCallback)
{
	if (_isRunning == true)
	{
		return;
	}
	_startTime = std::chrono::steady_clock::now();
	_isRunning = true;

	_woker = std::thread([=]
		{
			while (_isRunning == true)
			{
				std::this_thread::sleep_for(milliseconds); 
				intervalCallback();
			}
		});
}
void Timer::Stop()
{
	_isRunning = false;
	_woker.join();
}

void Timer::Reset()
{
	_startTime = std::chrono::steady_clock::now();
}

long long Timer::GetElapsedTime()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(current - _startTime).count();
}