#ifndef __ModuleTimeManager_H__
#define __ModuleTimeManager_H__

#include "Globals.h"
#include "Module.h"
#include "Timer.h"

class Application;

class ModuleTimeManager : public Module
{
public:
	ModuleTimeManager();
	~ModuleTimeManager();
	
	//Core
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	//Methods
	void computeDeltaTimes();
	float GetGameTime();
	float GetRealGameTime();
	float GetDeltaTime();
	float GetRealDeltaTime();

	//Variables
	unsigned int frameCount = 0;
	float timeScale = 1.0f;

private:

	Timer myTimer;

	
	float gameTime;
	float deltaTime;
	float realGameTime;
	float realDeltaTime;

};

#endif __ModuleTimeManager_H__