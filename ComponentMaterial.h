#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Globals.h"
#include "Component.h"
#include "ModuleTexture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* go);
	ComponentMaterial(GameObject* go, ComponentMaterial* comp);
	~ComponentMaterial();

	void Update();

	//Saving and loading
	void OnSave(SceneLoader & loader);
	void OnLoad(SceneLoader & loader);

	std::vector<Texture*> textures;
};

#endif __ComponentMaterial_H__
