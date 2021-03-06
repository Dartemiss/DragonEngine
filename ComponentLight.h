#ifndef __ComponentLight_H__
#define __ComponentLight_H__

#include "Component.h"
#include "MathGeoLib/Math/float3.h"

enum LightType
{
	LDIRECTIONAL = 0//,
	//LPOINT,
	//LSPOT
};

class ComponentLight : public Component
{
public:
	ComponentLight(GameObject* go);
	ComponentLight(GameObject* go, ComponentLight* comp);
	~ComponentLight();

	void DrawInspector();

	void SetDrawLightsForMeshes(const unsigned int program);
	void Draw();

	//Saving and loading
	void OnSave(SceneLoader & loader);
	void OnLoad(SceneLoader & loader);

private:
	LightType lightType = LDIRECTIONAL;

	float azimuth = 0.0f;
	float polar = 0.0f;
	float3 direction;
	float3 color = float3(1.0f, 1.0f, 1.0f);
	//TODO: be able to change light type with dropdown on Inspector
	//TODO: be able to change light color on Inspector

	void CalculateDirection();
};

#endif __ComponentLight_H__