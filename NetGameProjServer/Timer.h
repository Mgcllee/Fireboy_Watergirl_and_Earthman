#pragma once
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
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
private:
	std::thread _woker;
	std::atomic_bool _isRunning;
	std::chrono::steady_clock::time_point _startTime;
};