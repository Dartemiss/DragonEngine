#include "Timer.h"


//60 FPS -> 16.6 ms for computing each frame

void Timer::StartTimer()
{
	initialTime = (float)SDL_GetTicks();
	currentTime = 0.0f;
	isActivated = true;
	
}


float Timer::ReadTimer() const
{
	if(isActivated)
	{
		return (float)SDL_GetTicks() - initialTime;
	}
	
	return currentTime;
}

float Timer::StopTimer()
{
	isActivated = false;
	currentTime = (float)SDL_GetTicks() - initialTime;

	return currentTime;
}
