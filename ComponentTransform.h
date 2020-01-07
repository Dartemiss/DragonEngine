#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"
#include "MathGeoLib/Math/float4x4.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* gameObject);
	ComponentTransform(GameObject* gameObject, ComponentTransform* comp);
	~ComponentTransform();

	void EulerToQuat();
	void QuatToEuler();
	void UpdateMatrices();
	void SetGlobalMatrix(const float4x4 &parentGlobal);
	void SetGlobalMatrix(const float4x4 &newGlobal, const float4x4 &parentGlobal);
	void SetLocalMatrix(const float4x4 &newParentGlobalMatrix);
	void TranslateTo(const float3 &newPos);
	void DrawInspector();

	//Saving and loading
	void OnSave(SceneLoader & loader);
	void OnLoad(SceneLoader & loader);

	//Variables
	//Local Transform
	float3 position = float3(0.0f, 0.0f, 0.0f);
	Quat rotation = Quat::identity;
	float3 eulerRotation = float3(0.0f, 0.0f, 0.0f);
	float3 scale = float3(1.0f, 1.0f, 1.0f);

	float4x4 localModelMatrix = float4x4::identity;
	float4x4 globalModelMatrix = float4x4::identity;

};

#endif __ComponentTransform_H__