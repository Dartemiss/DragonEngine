#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject()
{
}

GameObject::GameObject(const char * name)
{
	this->name = name;
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
			break;
		case MESH:
			component = new ComponentMesh();
			break;
		case MATERIAL:
			component = new ComponentMaterial();
			break;
		default:
			LOG("ERROR: INVALID TYPE OF COMPONENT");
			return nullptr;
	}

	component->myGameObject = this;

	components.push_back(component);


	return component;
}
