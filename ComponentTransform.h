#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "Globals.h"
#include "GameObject.h"
#include "include/Math/float3.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	//Variables
	//Local Transform
	float3 position = float3(0.0f, 0.0f, 0.0f);
	float3 rotation = float3(0.0f, 0.0f, 0.0f);
	float3 scale = float3(0.0f, 0.0f, 0.0f);

};

#endif __ComponentTransform_H__