#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	//Core
	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	//Methods
	GameObject* CreateGameObject();
	GameObject* CreateGameObject(const char* name, GameObject* parent);
	void LoadModel(const char* path, GameObject* parent);

	void SelectObjectInHierarchy(GameObject* selected);
	//Drawing Methods
	void DrawUIBarMenuGameObject();
	void DrawGUI();
	//AllGameObjectsCreated
	std::vector<GameObject*> allGameObjects;
	bool showHierarchy = true;
	bool showInspector = true;
	//Game's Main Camera Object
	GameObject* mainCamera = nullptr;

private:
	//Root
	GameObject* root = nullptr;
	GameObject* selectedByHierarchy = nullptr;
	//GameObjects Counter
	unsigned int numberOfGameObjects = 0;


	
	

};

#endif __ModuleScene_H__