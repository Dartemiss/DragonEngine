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

	//Core
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	//Methods
	void SetFOV();
	void SetAspectRatio();
	void Rotate(float dx, float dy);
	void Move(float3 direction);
	void Orbit(float dx, float dy);
	void TranslateCameraToPoint(float3 &newPos);
	void SetNearPlaneDistance(float nearDist);
	void SetFarPlaneDistance(float farDist);
	void LookAt(float3 target);

	//Variables
	float aspect = 1.0f;
	Frustum *frustum;
	int wheelMovement;


	float movementSpeed = 0.2f;
	float rotationSpeed = 0.015f;
	float zoomSpeed = 0.1f;

};

#endif __ModuleCamera_H_