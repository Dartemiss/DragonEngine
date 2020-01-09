#include "ComponentLight.h"

ComponentLight::ComponentLight(GameObject * go)
{
	myGameObject = go;
	myType = LIGHT;
}

ComponentLight::ComponentLight(GameObject * go, ComponentLight * comp)
{
	myGameObject = go;
	myType = LIGHT;
}

ComponentLight::~ComponentLight()
{
}
