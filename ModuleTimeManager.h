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
	void InitDeltaTimes();
	void FinalDeltaTimes();
	float GetGameTime();
	float GetRealGameTime();
	float GetDeltaTime();
	float GetRealDeltaTime();
	void PauseGame();
	void UnPauseGame();
	void ExecuteNextFrames(int numberFrames);

	//Variables
	long long frameCount = 0;
	int FPS = 60;
	float timeScale = 1.0f;
	bool isPaused = false;
	bool waitingToPause = false;

private:

	Timer* realTimer;
	
	float gameTime = 0.0f;
	float deltaTime = 0.0f;
	float realGameTime = 0.0f;
	float realDeltaTime = 0.0f;

	float initialGameFrameTime = 0.0f;
	float initialRealFrameTime = 0.0f;

	float timePaused = 0.0f;

	int framesToPause = 0;

	unsigned int counterFPS = 0;
	float counterTimeFPS = 0.0f;
};

#endif __ModuleTimeManager_H__