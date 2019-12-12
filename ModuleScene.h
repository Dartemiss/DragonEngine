#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"
#include "Timer.h"
#include <set>

class MyQuadTree;
class AABBTree;

enum ShapeType
{
	OTHER = 0,
	SPHERE,
	CUBE,
	CYLINDER,
	TORUS
	
};

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
	GameObject* CreateGameObject(GameObject* go);

	void LoadModel(const char* path, GameObject* parent);

	//Creators
	void CreateEmpy(GameObject* parent);
	void CreateGameObjectBakerHouse(GameObject* parent);
	void CreateGameObjectShape(GameObject* parent, ShapeType shape);

	//Delete
	void RemoveGameObject(GameObject* go);

	void SelectObjectInHierarchy(GameObject* selected);
	//Drawing Methods
	void DrawUIBarMenuGameObject();
	void DrawGUI();

	//AllGameObjectsCreated
	std::set<GameObject*> allGameObjects;
	bool showHierarchy = true;
	bool showInspector = true;
	//Game's Main Camera Object
	GameObject* mainCamera = nullptr;

	//QuadTree
	MyQuadTree* quadtree = nullptr;
	MyQuadTree* quadtreeIterative = nullptr;
	AABBTree* aabbTree = nullptr;

	void AddToQuadtree(GameObject* go) const;
	void RemoveFromQuadTree(GameObject* go) const;

	bool quadTreeInitialized = false;

	//Static objects
	void BuildQuadTree();
	//Dynamic objects
	void BuildAABBTree();
	void CreateCubesScript();
	void CreateShapesScript();

	AABB* ComputeSceneAABB() const;

	//Move Objects
	void MoveObjects(GameObject* go) const;

	//Timers
	Timer iterative = Timer();
	Timer recursive = Timer();
	Timer aabbTreeTimer = Timer();

	float timeRecursive = 0.0f;
	float timeIterative = 0.0f;
	float timeAABBTree = 0.0f;

	bool quadtreeIsComputed = false;
	bool aabbTreeIsComputed = false;

	bool moveObjectsArround = false;

	bool moveItems = false;

	void SaveScene();
	void LoadScene();

	GameObject* clipboard = nullptr;
	void PasteGameObject(GameObject* go);

	void InsertChilds(GameObject* go);



private:
	//Root
	GameObject* root = nullptr;
	GameObject* selectedByHierarchy = nullptr;
	//GameObjects Counter
	unsigned int numberOfGameObjects = 0;
	unsigned int numberOfBakerHouse = 0;
	unsigned int numberOfSphere = 0;
	unsigned int numberOfCube = 0;
	unsigned int numberOfTorus = 0;
	unsigned int numberOfCylinder = 0;


	
	

};

#endif __ModuleScene_H__