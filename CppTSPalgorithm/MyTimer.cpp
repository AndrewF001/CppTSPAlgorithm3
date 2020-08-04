#include "MyTimer.h"

void MyTimer::Reset()
{
	Start = std::chrono::high_resolution_clock::now();
}

std::chrono::steady_clock::duration MyTimer::Time()
{
	return std::chrono::high_resolution_clock::now() - Start;
}