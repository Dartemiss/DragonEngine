#ifndef __ComponentLight_H__
#define __ComponentLight_H__

#include "Component.h"

class ComponentLight : public Component
{
public:
	ComponentLight(GameObject* go);
	ComponentLight(GameObject* go, ComponentLight* comp);
	~ComponentLight();
};

#endif __ComponentLight_H__