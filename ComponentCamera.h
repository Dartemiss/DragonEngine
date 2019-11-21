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
	
	float3 oldPosition;

	float Hnear;
	float Wnear;

	//Then we do the same for the far plane
	float Hfar;
	float Wfar;

	//Now we get the center of the planes
	float3 centerNear;
	float3 centerFar;

	//And now we get our points
	float3 NearTopLeft;
	float3 NearTopRight;
	float3 NearBottomLeft;
	float3 NearBottomRight;

	float3 FarTopLeft;
	float3 FarTopRight;
	float3 FarBottomLeft;
	float3 FarBottomRight;

};



#endif __ComponentCamera_H__
