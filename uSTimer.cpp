#include "uSTimer.h"
#include "SDL.h"

const static float frequency = (float)SDL_GetPerformanceFrequency();

void uSTimer::StartTimer()
{
	initialTime = (float)SDL_GetPerformanceCounter();
}

void uSTimer::Update()
{
	float now = (float) SDL_GetPerformanceCounter();
	currentTime = (float)(now - initialTime)*1000 / frequency;
}

float uSTimer::ReadTimer()
{
	Update();
	return currentTime;
}

float uSTimer::StopTimer()
{
	Update();
	return currentTime;
}
