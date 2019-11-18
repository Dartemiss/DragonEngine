#include "ModuleTimeManager.h"



ModuleTimeManager::ModuleTimeManager()
{
	realTimer = new Timer();
}


ModuleTimeManager::~ModuleTimeManager()
{
	delete realTimer;
}

bool ModuleTimeManager::Init()
{
	realTimer->StartTimer();

	return true;
}

update_status ModuleTimeManager::PreUpdate()
{
	if(waitingToPause && frameCount >= framesToPause)
	{
		PauseGame();
		waitingToPause = false;
		framesToPause = 0;	
	}

	return UPDATE_CONTINUE;
}

update_status ModuleTimeManager::Update()
{
	if(!isPaused)
	{
		gameTime += (realTimer->ReadTimer() - realGameTime) * timeScale;
	}
	
	realGameTime = realTimer->ReadTimer();

	return UPDATE_CONTINUE;
}

update_status ModuleTimeManager::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTimeManager::CleanUp()
{
	return true;
}

void ModuleTimeManager::InitDeltaTimes()
{
	initialGameFrameTime = gameTime + (realTimer->ReadTimer() - realGameTime) * timeScale;
	initialRealFrameTime = realTimer->ReadTimer();
}

void ModuleTimeManager::FinalDeltaTimes()
{
	if(!isPaused)
	{
		deltaTime = (gameTime + (realTimer->ReadTimer() - realGameTime) * timeScale) - initialGameFrameTime;
	}
	realDeltaTime = realTimer->ReadTimer()- initialRealFrameTime;

	float timeToWait = 0.0f;
	if(fixFPS)
	{
		float timeperFrame = (1000.0f / fixedFPS);
		timeToWait = timeperFrame - realDeltaTime;

		if(timeToWait > 0.0f)
		{
			Wait(timeToWait);
		}
	}

	counterTimeFPS += realDeltaTime + timeToWait;
	++counterFPS;

	if(counterTimeFPS > 1000.0f)
	{
		FPS = counterFPS;
		counterFPS = 0;
		counterTimeFPS = 0.0f;
	}

}

float ModuleTimeManager::GetGameTime()
{
	return gameTime;
}

float ModuleTimeManager::GetRealGameTime()
{
	return realGameTime;
}

float ModuleTimeManager::GetDeltaTime()
{
	return deltaTime;
}

float ModuleTimeManager::GetRealDeltaTime()
{
	return realDeltaTime;
}

void ModuleTimeManager::PauseGame()
{
	isPaused = true;
}

void ModuleTimeManager::UnPauseGame()
{
	isPaused = false;
}

void ModuleTimeManager::ExecuteNextFrames(int numberFrames)
{
	if(isPaused)
	{
		waitingToPause = true;
		UnPauseGame();
		framesToPause = frameCount + numberFrames;
		return;
	}

	LOG("Cannot execute next frames if game is not paused.");
	return;

}

void ModuleTimeManager::Wait(float timeToWait)
{
	SDL_Delay(timeToWait);
}
