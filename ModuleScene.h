#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"
#include "Timer.h"
#include "Point.h"
#include "imgui/imgui.h"
#include "MathGeoLib/Math/float2.h"
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
	void CreateEmpty(GameObject* parent);
	void CreateGameObjectBakerHouse(GameObject* parent);
	void CreateGameObjectZomBunny(GameObject* parent);
	void CreateGameObjectShape(GameObject* parent, ShapeType shape);

	//Delete
	void RemoveGameObject(GameObject* go);

	void SelectObjectInHierarchy(GameObject* selected);
	//Drawing Methods
	void DrawGUI();

	//AllGameObjectsCreated
	std::set<GameObject*> allGameObjects;
	std::set<GameObject*> staticGO;
	std::set<GameObject*> dynamicGO;
	bool showHierarchy = true;
	bool showInspector = true;

	//Game's Main Camera Object
	GameObject* mainCamera = nullptr;

	//Directional light
	GameObject* directionalLight = nullptr;

	//QuadTree
	MyQuadTree* quadtree = nullptr;
	AABBTree* aabbTree = nullptr;

	void AddToQuadtree(GameObject* go) const;

	bool quadTreeInitialized = false;

	//Static objects
	void BuildQuadTree();
	//Dynamic objects
	void BuildAABBTree();
	void CreateCubesScript();
	void CreateShapesScript();
	void CreateHousesScript();

	AABB* ComputeSceneAABB() const;

	//Move Objects
	void MoveObjects(GameObject* go) const;

	//Timers
	Timer iterative = Timer();
	Timer aabbTreeTimer = Timer();

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
	void DuplicateGameObject(GameObject* go);
	void InsertChilds(GameObject* go);

	GameObject* selectedByHierarchy = nullptr;

	//Mouse Picking
	LineSegment* CreateRayCast(float3 origin, float3 direction, float maxDistance);
	GameObject* IntersectRayCast(float3 origin, const LineSegment &ray);
	LineSegment* CreateRayCast(float normalizedX, float normalizedY) const;
	LineSegment* currentRay = nullptr;

	void PickObject(const ImVec2 &sizeWindow, const ImVec2 &posWindow);

	GameObject* GetRoot() const;


private:
	//Root
	GameObject* root = nullptr;
	//GameObjects Counter
	unsigned int numberOfGameObjects = 0;
	unsigned int numberOfBakerHouse = 0;
	unsigned int numberOfSphere = 0;
	unsigned int numberOfCube = 0;
	unsigned int numberOfTorus = 0;
	unsigned int numberOfCylinder = 0;

	float2 offset = float2(-5.0f, -25.0f);

	
	

};

#endif __ModuleScene_H__