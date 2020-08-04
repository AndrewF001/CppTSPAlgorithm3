#pragma once
#include <chrono>
class MyTimer
{
public:
	MyTimer()
	{
		Start = std::chrono::high_resolution_clock::now();
	};
	std::chrono::steady_clock::duration Time();
	void Reset();
private:
	std::chrono::steady_clock::time_point Start;
	
};

