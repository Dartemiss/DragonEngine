#ifndef __ModuleIMGUI_H__
#define __ModuleIMGUI_H__

#include "Module.h"
#include "Globals.h"
#include "GUIConsole.h"
#include "GUIAbout.h"


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


};
#endif // __ModuleIMGUI_H__
