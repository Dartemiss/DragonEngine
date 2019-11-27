#ifndef __GameObject_H__
#define __GameObject_H__

#include "Globals.h"
#include "Component.h"
#include "include/Geometry/AABB.h"
#include <string>
#include <vector>


//Class declaration is way faster than include header
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

class GameObject
{
public:
	GameObject();
	GameObject(const char* name);
	~GameObject();

	//Core
	void Update();
	void SetParent(GameObject* newParent);
	void RemoveChildren(GameObject* child);

	//Component Creation
	Component* CreateComponent(ComponentType type);

	//Hierarchy
	void DrawHierarchy(GameObject* selected);

	//Draw Main Camera
	void DrawCamera();

	//Update
	void UpdateTransform();

	//Variables
	//ID are unique
	//TODO: ID system
	unsigned int ID;

	//All GameObjects have a transform
	ComponentTransform* myTransform = nullptr;
	ComponentMesh* myMesh = nullptr;
	ComponentMaterial* myMaterial = nullptr;

	//Parent
	GameObject* parent = nullptr;

	//Chilren
	std::vector<GameObject*> children;

	//Components assigned to gameObject
	std::vector<Component*> components;
	void SetName(const std::string &newName);
	
	//ID substitute
	bool isRoot = false;
	bool isEnabled = true;
	bool isStatic = false;

	//Compute
	void ComputeAABB();
	void DrawAABB() const;

	AABB* boundingBox = nullptr;

	void DrawInspector(bool &showInspector);

private:
	std::string name;

	void CheckDragAndDrop(GameObject* go);


};

#endif __GameObject_H__