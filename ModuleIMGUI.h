#ifndef __ModuleIMGUI_H__
#define __ModuleIMGUI_H__

#include "Module.h"
#include "Globals.h"
#include "GUIConsole.h"
#include "GUIAbout.h"
#include "GUIWindow.h"
#include "GUICamera.h"
#include "GUITime.h"
#include "GUIInspector.h"

class ModuleIMGUI : public Module
{
public:
	ModuleIMGUI();
	~ModuleIMGUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	GUIConsole console;
	GUIAbout about;
	GUIWindow guiWindow;
	GUICamera guiCamera;
	GUITime timeManager;
	GUIInspector inspector;

private:
	bool show_demo_window = false;
	bool scrollDownConsole = false;
	bool openAbout = false;
	bool showWindowConfig = false;
	bool showCameraGUI = false;
	bool showTimers = false;
	bool showInspector = false;
};
#endif // __ModuleIMGUI_H__
