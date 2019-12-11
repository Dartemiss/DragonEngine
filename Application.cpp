#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleIMGUI.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleTimeManager.h"
#include "ModuleScene.h"
#include "ModuleDebugDraw.h"
#include "Timer.h"
#include "uSTimer.h"
#include "Dependencies/brofiler/Brofiler.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(timemanager = new ModuleTimeManager());
	modules.push_back(imgui = new ModuleIMGUI());
	modules.push_back(input = new ModuleInput());
	modules.push_back(modelLoader = new ModuleModelLoader());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(debugDraw = new ModuleDebugDraw());


}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	uSTimer initTimer;
	initTimer.StartTimer();

	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	float time = initTimer.StopTimer() / 1000.0f;
	LOG("Total Time of init is: %.5f seconds", time);

	return ret;
}

update_status Application::Update()
{
	//Begining of the frame
	BROFILER_FRAME("Mainframe");
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
