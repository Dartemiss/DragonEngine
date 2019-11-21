#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Globals.h"
#include "Component.h"
#include "ModuleTexture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	void Update();

	std::vector<Texture*> textures;

};

#endif __ComponentMaterial_H__
