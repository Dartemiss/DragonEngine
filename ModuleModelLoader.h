#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Globals.h"
#include "Module.h"

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

};
#endif __ModuleModelLoader_h__
