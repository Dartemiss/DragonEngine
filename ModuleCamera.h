#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float4x4.h"

class Application;
class ComponentCamera;

class ModuleCamera : public Module
{
public:
	//Methods
	ModuleCamera();
	~ModuleCamera();

	//Core
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	ComponentCamera* editorCamera = nullptr;

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

	//Getter Matrix
	float4x4 GetProjMatrix() const;
	float4x4 GetViewMatrix() const;


	float movementSpeed = 0.2f;
	float rotationSpeed = 0.015f;
	float zoomSpeed = 0.5f;
	float motionOffset = 2.5f;

	bool SceneNotActive = true;

};

#endif __ModuleCamera_H_