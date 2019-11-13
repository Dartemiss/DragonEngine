#include "ModuleTimeManager.h"



ModuleTimeManager::ModuleTimeManager()
{
	realTimer = new Timer();
}


ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Init()
{
	realTimer->StartTimer();

	return true;
}

update_status ModuleTimeManager::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimeManager::Update()
{
	gameTime += (realTimer->ReadTimer() - realGameTime) * timeScale;
	realGameTime = realTimer->ReadTimer();

	//LOG("Time: %.3f ms", realGameTime);

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
	deltaTime = (gameTime + (realTimer->ReadTimer() - realGameTime) * timeScale) - initialGameFrameTime;
	realDeltaTime = realTimer->ReadTimer()- initialRealFrameTime;

	counterTimeFPS += realDeltaTime;
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
