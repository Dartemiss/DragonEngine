#ifndef __ModuleIMGUI_H__
#define __ModuleIMGUI_H__

#include "Module.h"
#include "Globals.h"
#include "GUIConsole.h"
#include "GUIAbout.h"
#include "GUIWindow.h"
#include "GUICamera.h"


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

private:
	bool show_demo_window;
	bool scrollDownConsole;
	bool openAbout;
	bool showWindowConfig;
	bool showCameraGUI;
};
#endif // __ModuleIMGUI_H__