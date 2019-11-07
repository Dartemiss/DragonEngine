#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "Globals.h"
#include "include/Geometry/Frustum.h"

class Application;

class ModuleCamera : public Module
{
public:
	//Methods
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void SetFOV(float FOV);
	void SetAspectRatio();


	//Variables
	float aspect = 1.0f;
	unsigned int width = SCREEN_WIDTH;
	unsigned int height = SCREEN_HEIGHT;
	Frustum *frustum;

};

#endif __ModuleCamera_H_