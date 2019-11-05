#ifndef __GUIWindow_H__
#define __GUIWindow_H__

#include "imgui/imgui.h"
#include "Globals.h"
#include "GUI.h"
#include <vector>
#include "ModuleTexture.h"
#include "ilu.h"

class GUIWindow : public GUI
{
public:
	GUIWindow();
	~GUIWindow();

	void Draw(const char* title, bool* p_opened, SDL_Window* window, ILinfo *texInfo);
	std::vector<float> fps_log;
	std::vector<float> ms_log;

private:
	bool fullscreen = FULLSCREEN;
	bool resizable = RESIZABLE;
	float currentTime;
	float deltaTime;
	float previousTime;
	float timeCounter;

};
#endif __GUIWindow_H__
