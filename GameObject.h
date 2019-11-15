#ifndef __GameObject_H__
#define __GameObject_H__

#include "Globals.h"
#include "Component.h"
#include <string>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	//Core
	void Update();

	//Component Creation
	Component* CreateComponent(ComponentType type);

private:
	std::string name;
	std::vector<Component*> components;
	//Parent
	GameObject* parent;
	//Chilren
	std::vector<GameObject*> children;

};

#endif __GameObject_H__