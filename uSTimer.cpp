#include "uSTimer.h"
#include "SDL.h"

const static float frequency = (float)SDL_GetPerformanceFrequency();

void uSTimer::StartTimer()
{
	isActivated = true;
	initialTime = (float)SDL_GetPerformanceCounter();
}


float uSTimer::ReadTimer() const
{
	if(isActivated)
	{
		return (float)((float)SDL_GetPerformanceCounter() - initialTime) * 1000 / frequency;
	}
	return currentTime;
}

float uSTimer::StopTimer()
{
	isActivated = false;
	currentTime = (float)((float)SDL_GetPerformanceCounter() - initialTime) * 1000 / frequency;
	return currentTime;
}
