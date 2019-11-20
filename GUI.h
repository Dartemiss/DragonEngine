#pragma once

#include "Globals.h"
#include "glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL.h"
#include "assert.h"

class ModuleGUI;

class GUI
{
public:

	GUI()
	{
	}

	virtual bool Init()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{

		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void SetEnable(bool enable)
	{
		isEnabled = enable;
		return;
	}

	virtual void ToggleEnable()
	{
		isEnabled = !isEnabled;
	}

	bool isEnabled = false;
};