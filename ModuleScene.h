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
	
	//Drawing Methods
	void DrawUIBarMenuGameObject();
	void DrawHierarchy();
	//AllGameObjectsCreated
	std::vector<GameObject*> allGameObjects;

private:
	//Root
	GameObject* root = nullptr;
	GameObject* selectedByHierarchy = nullptr;
	//GameObjects Counter
	unsigned int numberOfGameObjects = 0;
	bool showHierarchy = true;

	
	

};

#endif __ModuleScene_H__