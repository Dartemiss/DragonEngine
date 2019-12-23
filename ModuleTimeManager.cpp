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
	if(!isPaused && isPlaying)
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
	if(!isPaused && isPlaying)
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

float ModuleTimeManager::GetGameTime() const
{
	return gameTime;
}

float ModuleTimeManager::GetRealGameTime() const
{
	return realGameTime;
}

float ModuleTimeManager::GetDeltaTime() const
{
	return deltaTime;
}

float ModuleTimeManager::GetRealDeltaTime() const
{
	return realDeltaTime;
}

void ModuleTimeManager::PauseGame()
{
	if(isPlaying)
		isPaused = !isPaused;
}

void ModuleTimeManager::ExecuteNextFrames(int numberFrames)
{
	if(isPaused)
	{
		waitingToPause = true;
		isPaused = false;
		framesToPause = frameCount + numberFrames;
		return;
	}

	LOG("Cannot execute next frames if game is not paused.");
	return;

}

void ModuleTimeManager::Wait(float timeToWait)
{
	SDL_Delay(static_cast<Uint32>(timeToWait));
}

void ModuleTimeManager::PlayGame()
{
	gameTime = 0.0f;
	isPaused = false;
	isPlaying = !isPlaying;
}
