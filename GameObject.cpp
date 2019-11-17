#include "GameObject.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject()
{
}

GameObject::GameObject(const char * name)
{
	this->name = name;
	CreateComponent(TRANSFORM);

}


GameObject::~GameObject()
{
}

void GameObject::Update()
{
	return;
}

void GameObject::SetParent(GameObject * newParent)
{
	if(newParent != nullptr)
	{
		LOG("Setting new GamesObject parent and children.")
		parent = newParent;
		parent->children.push_back(this);
	}

	LOG("ERROR: Cannot set parent because new Parent is nullptr.");
	return;

}

Component * GameObject::CreateComponent(ComponentType type)
{
	Component* component = nullptr;

	switch(type)
	{
		case TRANSFORM:
			component = new ComponentTransform();
			myTransform = (ComponentTransform*)component;
			break;
		case MESH:
			component = new ComponentMesh();
			myMeshes = (ComponentMesh*)component;
			break;
		case MATERIAL:
			component = new ComponentMaterial();
			myMaterial = (ComponentMaterial*)component;
			break;
		default:
			LOG("ERROR: INVALID TYPE OF COMPONENT");
			return nullptr;
			break;
	}

	component->myGameObject = this;

	components.push_back(component);


	return component;
}

void GameObject::LoadModel(const char * path)
{
	if(myMeshes == nullptr || myMaterial == nullptr)
	{
		LOG("ERROR: Cannot load a model without a Mesh and a Material components");
		return;
	}

	myMeshes->LoadMeshes(path);

}
