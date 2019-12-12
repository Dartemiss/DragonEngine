#include "ComponentMaterial.h"
#include "SceneLoader.h"


ComponentMaterial::ComponentMaterial(GameObject* go)
{
	myGameObject = go;
	myType = MATERIAL;
}

ComponentMaterial::ComponentMaterial(GameObject * go, ComponentMaterial * comp)
{
	myGameObject = go;
	myType = MATERIAL;
	//TODO: When this component is done this method have to copy all comp relevant data to this object

}


ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Update()
{
	return;
}

bool ComponentMaterial::CleanUp()
{

	return false;
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
