#ifndef __GUIWindow_H__
#define __GUIWindow_H__

#include "Globals.h"
#include "GUI.h"
#include "ModuleTexture.h"
#include "Timer.h"
#include "imgui/imgui.h"
#include "ilu.h"
#include <vector>

class Application;

class GUIWindow : public GUI
{
public:
	GUIWindow();
	~GUIWindow() = default;

	void Draw(const char* title, bool* p_opened);
	std::vector<float> fps_log;
	std::vector<float> ms_log;

private:
	float currentTime;
	float deltaTime;
	float previousTime;
	float timeCounter;
	float actualBright = 1.000f;

	Timer fpsTimer;


};
#endif __GUIWindow_H__
