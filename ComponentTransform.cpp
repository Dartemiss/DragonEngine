#include "ComponentTransform.h"



ComponentTransform::ComponentTransform()
{
	myType = TRANSFORM;
	UpdateMatrices();
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::UpdateMatrices()
{
	globalModelMatrix = globalModelMatrix * localModelMatrix.Inverted();
	localModelMatrix = float4x4::FromTRS(position, rotation, scale);
	globalModelMatrix = globalModelMatrix * localModelMatrix;
}
