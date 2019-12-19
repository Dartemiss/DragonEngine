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

	//Values Scene
	float scenePosRatioWidth = 0.183f;
	float scenePosRatioHeight = 0.047f;

	float sceneSizeRatioWidth = 0.626f;
	float sceneSizeRatioHeight = 0.709f;

	//Values Inspector
	float inspectorPosRatioWidth = 0.810;
	float inspectorPosRatioHeight = 0.017f;

	float inspectorSizeRatioWidth = 0.186f;
	float inspectorSizeRatioHeight = 0.98f;

	//Values Console
	float consolePosRatioHeight = 0.757f;

	float consoleSizeRatioWidth = 0.457f;
	float consoleSizeRatioHeight = 0.239f;

	//Values Timers
	float timerPosRatioWidth = 0.459f;
	float timerPosRatioHeight = 0.757f;

	float timerSizeRatioWidth = 0.350f;
	float timerSizeRatioHeight = 0.239f;

	//Values Tabs
	float tabsPosRatioWidth = 0.183f;
	float tabsPosRatioHeight = 0.018f;

	float tabsSizeRatioWidth = 0.628f;
	float tabsSizeRatioHeight = 0.741f;


	//Values hierarchy
	float hierarchySizeRatioWidth = 0.185f;
	float hierarchySizeRatioHeight = 0.745f;



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
