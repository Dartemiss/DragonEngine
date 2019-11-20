#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "Globals.h"
#include "include/Geometry/Frustum.h"
#include "include/Math/float4x4.h"

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
	void SetAspectRatio(int newHeight, int newWidth);
	void Rotate(const float dx, const float dy);
	void Move(float3 direction);
	void Orbit(const float dx,const float dy);
	void Zoom(const bool direction);
	void TranslateCameraToPoint(const float3 &newPos);
	void SetNearPlaneDistance(const float nearDist);
	void SetFarPlaneDistance(const float farDist);
	void LookAt(const float3 target);

	//Update Projection and View matrices
	void UpdateUniformShaderMatrices();

	//Variables
	float aspect = 1.0f;
	Frustum *frustum;

	//Matrices
	float4x4 proj = float4x4::zero;
	float4x4 view = float4x4::zero;

	float movementSpeed = 0.2f;
	float rotationSpeed = 0.015f;
	float zoomSpeed = 0.5f;
	float motionOffset = 2.5f;

};

#endif __ModuleCamera_H_