#include "ModuleTimeManager.h"



ModuleTimeManager::ModuleTimeManager()
{
}


ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Init()
{
	myTimer.StartTimer();

	return true;
}

update_status ModuleTimeManager::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimeManager::Update()
{
	gameTime = myTimer.ReadTimer() * timeScale;
	realGameTime = myTimer.ReadTimer();

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

void ModuleTimeManager::computeDeltaTimes()
{
	float delta = myTimer.ReadTimer() * timeScale;
	deltaTime = delta - deltaTime;

	float realDelta = myTimer.ReadTimer();
	realDeltaTime = realDelta - realDeltaTime;
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
