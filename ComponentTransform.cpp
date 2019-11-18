#include "ComponentTransform.h"
#include "include/Math/MathFunc.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* gameObject)
{
	myGameObject = gameObject;
	myType = TRANSFORM;
	UpdateMatrices();
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::EulerToQuat()
{
	rotation = rotation.FromEulerXYX(DegToRad(eulerRotation).x, DegToRad(eulerRotation).y, DegToRad(eulerRotation).z);
}

void ComponentTransform::UpdateMatrices()
{
	globalModelMatrix = globalModelMatrix * localModelMatrix.Inverted();
	localModelMatrix = float4x4::FromTRS(position, rotation, scale);
	globalModelMatrix = globalModelMatrix * localModelMatrix;
}

void ComponentTransform::SetGlobalMatrix(float4x4 &parentGlobal)
{
	globalModelMatrix = parentGlobal * localModelMatrix;
}
