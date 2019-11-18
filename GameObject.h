#ifndef __GameObject_H__
#define __GameObject_H__

#include "Globals.h"
#include "Component.h"
#include <string>
#include <vector>

class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;


class GameObject
{
public:
	GameObject();
	GameObject(const char* name);
	~GameObject();

	//Core
	void Update();
	void SetParent(GameObject* newParent);

	//Component Creation
	Component* CreateComponent(ComponentType type);
	void LoadModel(const char* name);

	//Hierarchy
	void DrawHierarchy(GameObject* selected);

	//Update
	void UpdateTransform();

	//Variables
	//ID are unique
	//TODO: ID system
	unsigned int ID;

	//All GameObjects have a transform
	ComponentTransform* myTransform = nullptr;
	ComponentMesh* myMeshes = nullptr;
	ComponentMaterial* myMaterial = nullptr;

private:
	std::string name;
	//Components assigned to gameObject
	std::vector<Component*> components;
	//Parent
	GameObject* parent;
	//Chilren
	std::vector<GameObject*> children;



};

#endif __GameObject_H__