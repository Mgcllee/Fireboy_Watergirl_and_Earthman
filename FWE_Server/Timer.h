#pragma once

#include "stdafx.h"
#include "Client.h"

#include <time.h>
#include <functional>


class Timer {
public :
	Timer(array<Client, 3>* clients);
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

	bool isTimeOut = false;
	bool gameEnd = false;

	double timeoutSeconds = 50;

private:
	array<Client, 3>* clients;

	std::thread woker;
	std::atomic_bool isRunning;
	std::chrono::steady_clock::time_point startTime;
};