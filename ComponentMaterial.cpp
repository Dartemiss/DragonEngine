#include "ComponentMaterial.h"
#include "SceneLoader.h"


ComponentMaterial::ComponentMaterial()
{
	myType = MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Update()
{
	return;
}

void ComponentMaterial::OnSave(SceneLoader & loader)
{
	loader.AddUnsignedInt("Type", myType);
	//TODO implement save
}

void ComponentMaterial::OnLoad(SceneLoader & loader)
{
	//TODO implement load
}
