#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__

#include "Globals.h"
#include "Component.h"
#include "include/Geometry/Frustum.h"
#include "include/Math/float4x4.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	void Update();

	//Methods
	void SetFOV();
	void SetAspectRatio(int newHeight, int newWidth);
	void Rotate(const float dx, const float dy);
	void Move(float3 direction);
	void TranslateCameraToPoint(const float3 &newPos);
	void SetNearPlaneDistance(const float nearDist);
	void SetFarPlaneDistance(const float farDist);
	void LookAt(const float3 target);

	//Drawing
	void DrawFrustum();

	//Variables
	float aspect = 1.0f;
	Frustum* frustum;

	//Matrices
	float4x4 proj = float4x4::zero;
	float4x4 view = float4x4::zero;

	float movementSpeed = 0.2f;
	float rotationSpeed = 0.015f;
	float zoomSpeed = 0.5f;
	float motionOffset = 2.5f;

	unsigned int frustumVAO = 0; 
	


};



#endif __ComponentCamera_H__
