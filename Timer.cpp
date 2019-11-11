#include "Timer.h"


//60 FPS -> 16.6 ms for computing each frame

void Timer::StartTimer()
{
	initialTime = (float)SDL_GetTicks();
	currentTime = 0.0f;
	isActivated = true;
	
}

void Timer::Update()
{
	if(isActivated)
	{
		float aux = (float)SDL_GetTicks();
		deltaTime = aux - currentTime - initialTime;
		currentTime += deltaTime;
	}
}

float Timer::ReadTimer()
{
	Update();
	return currentTime;
}

float Timer::StopTimer()
{
	Update();
	isActivated = false;
	initialTime = 0.0f;

	return currentTime;
}
