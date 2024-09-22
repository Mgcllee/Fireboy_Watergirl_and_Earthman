#pragma once
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <time.h>
class Timer
{
public :
	Timer();
	~Timer();
	typedef std::chrono::milliseconds Milliseconds;
	typedef std::function<void(void)> IntervalCallback;
public :
	void Start(const Milliseconds& milliseconds, const IntervalCallback& intervalCallback);
	void Stop();
	bool IsRunning();
	long long GetElapsedTime();
	void Reset();
	void start_timer();
	bool time_out = false;
private:
	std::thread woker;
	std::atomic_bool isRunning;
	std::chrono::steady_clock::time_point startTime;
};