#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Component.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/float4.h"
#include <vector>

struct Texture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* go);
	ComponentMaterial(GameObject* go, ComponentMaterial* comp);
	~ComponentMaterial();

	void Update();
	bool CleanUp();

	void SetTextures(std::vector<Texture> & textures);
	void SetDrawTextures(const unsigned int program);

	//Saving and loading
	void OnSave(SceneLoader & loader);
	void OnLoad(SceneLoader & loader);

	float kDiffuse;
	float kSpecular;
	float KAmbient;
	float shininess;

	float4 diffuseColor;
	float3 specularColor;
	float3 emissiveColor;

	Texture * diffuseMap = nullptr;
	Texture * specularMap = nullptr;
	Texture * occlusionMap = nullptr;
	Texture * emissiveMap = nullptr;
};

#endif __ComponentMaterial_H__
