#include "ComponentTransform.h"
#include "MathGeoLib/Math/MathFunc.h"
#include "GameObject.h"
#include "SceneLoader.h"

ComponentTransform::ComponentTransform(GameObject* gameObject)
{
	myGameObject = gameObject;
	myType = TRANSFORM;
	UpdateMatrices();
}

ComponentTransform::ComponentTransform(GameObject * gameObject, ComponentTransform * comp)
{
	myGameObject = gameObject;
	myType = TRANSFORM;
	position = comp->position;
	rotation = comp->rotation;
	scale = comp->scale;
	UpdateMatrices();
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::EulerToQuat()
{
	rotation = rotation.FromEulerXYX(DegToRad(eulerRotation).x, DegToRad(eulerRotation).y, DegToRad(eulerRotation).z);
}

void ComponentTransform::QuatToEuler()
{
	eulerRotation = rotation.ToEulerXYZ();
}

void ComponentTransform::UpdateMatrices()
{
	globalModelMatrix = globalModelMatrix * localModelMatrix.Inverted();
	localModelMatrix = float4x4::FromTRS(position, rotation, scale);
	globalModelMatrix = globalModelMatrix * localModelMatrix;
}

void ComponentTransform::SetGlobalMatrix(const float4x4 &parentGlobal)
{
	globalModelMatrix = parentGlobal * localModelMatrix;
}

void ComponentTransform::SetGlobalMatrix(const float4x4 & newGlobal, const float4x4 & parentGlobal)
{
	globalModelMatrix = newGlobal;
	SetLocalMatrix(parentGlobal);
}

void ComponentTransform::SetLocalMatrix(const float4x4 &newParentGlobalMatrix)
{
	localModelMatrix = newParentGlobalMatrix.Inverted() *  globalModelMatrix;
	localModelMatrix.Decompose(position, rotation, scale);
	
	if (scale.x < 0.01f)
		scale.x = 0.01f;

	if (scale.y < 0.01f)
		scale.y = 0.01f;
	
	if (scale.z < 0.01f)
		scale.z = 0.01f;

	QuatToEuler();
}

void ComponentTransform::TranslateTo(const float3 & newPos)
{
	position = newPos;
}

void ComponentTransform::OnSave(SceneLoader & loader)
{
	loader.AddVec3f("Translation", position);
	loader.AddVec3f("Scale", scale);
	loader.AddVec4f("Rotation", float4(rotation.x, rotation.y, rotation.z, rotation.w));
}

void ComponentTransform::OnLoad(SceneLoader & loader)
{
	position = loader.GetVec3f("Translation", float3(0, 0, 0));
	scale = loader.GetVec3f("Scale", float3(1, 1, 1));
	float4 rotationVec = loader.GetVec4f("Rotation", float4(0, 0, 0, 1));
	rotation = Quat(rotationVec.x, rotationVec.y, rotationVec.z, rotationVec.w);

	QuatToEuler();
	UpdateMatrices();
}


