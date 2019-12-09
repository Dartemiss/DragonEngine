#include "ModuleScene.h"
#include "ModuleTimeManager.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "Timer.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "MyQuadTree.h"
#include "AABBTree.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "include/Math/float4.h"
#include <random>


ModuleScene::ModuleScene()
{
	root = new GameObject("World");
	root->isRoot = true;
	root->isStatic = true;
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	//Creating the main camera of the game
	mainCamera = CreateGameObject("Main Camera", root);
	mainCamera->CreateComponent(CAMERA);

	allGameObjects.insert(mainCamera);

	quadtree = new MyQuadTree(AABB(float3(-100,0,-40), float3(100,0,100)),1);
	quadtreeIterative = new MyQuadTree(new AABB(float3(-100, 0, -100), float3(100, 0, 100)));
	aabbTree = new AABBTree(20);

	return true;
}

update_status ModuleScene::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update()
{
	for(auto gameObject : allGameObjects)
	{
		gameObject->UpdateTransform();
		gameObject->Update();

		if(aabbTreeIsComputed && gameObject->globalBoundingBox != nullptr)
		{
			aabbTree->UpdateObject(gameObject);
		}

		if (moveObjectsArround)
			MoveObjects(gameObject);
	}

	DrawGUI();
	
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	for(auto GO : allGameObjects)
	{
		delete GO;
	}

	delete root;

	return true;
}

GameObject * ModuleScene::CreateGameObject()
{
	std::string defaultName = "NewGameObject" + std::to_string(numberOfGameObjects);
	GameObject* gameObject = new GameObject(defaultName.c_str());
	gameObject->SetParent(root);

	LOG("Creating new GameObject with name: %s", defaultName);

	++numberOfGameObjects;

	selectedByHierarchy = gameObject;

	return gameObject;
}

GameObject * ModuleScene::CreateGameObject(const char * name, GameObject * parent)
{
	GameObject* gameObject = new GameObject(name);
	gameObject->SetParent(parent);

	LOG("Creating new GameObject with name: %s", name);

	++numberOfGameObjects;

	selectedByHierarchy = gameObject;

	return gameObject;
}

void ModuleScene::LoadModel(const char * path, GameObject* parent)
{
	LOG("Trying to load model in path : %s", path);
	App->modelLoader->loadModel(path);

	int numObject = 0;
	std::string name = App->modelLoader->nameOfModel;
	LOG("Creating parent gameObject %s", name.c_str());
	parent->SetName(name);

	LOG("For each mesh of the model we create a gameObject.");
	for(auto mesh : App->modelLoader->meshes)
	{
		std::string newName = name + std::to_string(numObject);
		GameObject* newMeshObject = CreateGameObject(newName.c_str(), parent);
		ComponentMesh* myMeshCreated = (ComponentMesh*)newMeshObject->CreateComponent(MESH);
		
		myMeshCreated->LoadMesh(mesh);
		newMeshObject->ComputeAABB();
		allGameObjects.insert(newMeshObject);

		++numObject;
	}

	LOG("Deliting info from ModelLoader");
	App->modelLoader->emptyScene();
	parent->ComputeAABB();

	//Setting parent as a meshParent
	parent->isParentOfMeshes = true;

	return;
}

void ModuleScene::CreateEmpy(GameObject* parent)
{
	std::string defaultName = "NewGameObject" + std::to_string(numberOfGameObjects + 1);
	GameObject* empty = CreateGameObject(defaultName.c_str(), parent);
	

	allGameObjects.insert(empty);

	return;
}

void ModuleScene::CreateGameObjectBakerHouse(GameObject * parent)
{
	if(parent == nullptr)
	{
		LOG("ERROR: Parent is nullptr, cannot create gameObject.");
		return;
	}

	LOG("Creating a GameObject with Baker House Mesh.");
	std::string defaultName = "BakerHouse" + std::to_string(numberOfBakerHouse + 1);
	GameObject* newGameObject = CreateGameObject(defaultName.c_str(), parent);
	LoadModel("../Models/baker_house/BakerHouse.fbx", newGameObject);
	++numberOfBakerHouse;

	allGameObjects.insert(newGameObject);
	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());

	if(quadTreeInitialized)
		AddToQuadtree(newGameObject);

	return;
}

void ModuleScene::CreateGameObjectShape(GameObject * parent, ShapeType shape)
{
	if (parent == nullptr)
	{
		LOG("ERROR: Parent is nullptr, cannot create gameObject.");
		return;
	}

	std::string defaultName;
	bool correct;
	switch (shape)
	{
	case SPHERE:
		LOG("Creating a GameObject with Sphere Mesh.");
		defaultName = "Sphere" + std::to_string(numberOfSphere + 1);
		correct = App->modelLoader->LoadSphere(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(0.4f, 0.4f, 0.4f, 0.4f));
		if (!correct)
		{
			LOG("ERROR: Cannot load the sphere mesh correctly.");
			return;
		}
		++numberOfSphere;
		break;
	case CUBE:
		LOG("Creating a GameObject with cube Mesh.");
		defaultName = "Cube" + std::to_string(numberOfCube + 1);
		correct = App->modelLoader->LoadCube("cube0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, float4(0.4f, 0.4f, 0.4f, 0.4f));
		if (!correct)
		{
			LOG("ERROR: Cannot load the cube mesh correctly.");
			return;
		}
		++numberOfCube;
		break;
	case CYLINDER:
		LOG("Creating a GameObject with Cylinder Mesh.");
		defaultName = "Cylinder" + std::to_string(numberOfCylinder + 1);
		correct = App->modelLoader->LoadCylinder(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(0.4f, 0.4f, 0.4f, 0.4f));
		if (!correct)
		{
			LOG("ERROR: Cannot load the cylinder mesh correctly.");
			return;
		}
		++numberOfCylinder;
		break;
	case TORUS:
		LOG("Creating a GameObject with torus Mesh.");
		defaultName = "Torus" + std::to_string(numberOfTorus + 1);
		correct = App->modelLoader->LoadTorus(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 0.5f, 0.67f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
		if (!correct)
		{
			LOG("ERROR: Cannot load the torus mesh correctly.");
			return;
		}
		++numberOfTorus;
		break;
	default:
		break;
	}


	GameObject* newGameObject = CreateGameObject(defaultName.c_str(), parent);
	
	newGameObject->shape = shape;

	if(!App->modelLoader->meshes.size() == 1)
	{
		LOG("ERROR: Sphere model cannot have more than one mesh. ");
		delete newGameObject;
		return;
	}



	ComponentMesh* myMeshCreated = (ComponentMesh*)newGameObject->CreateComponent(MESH);
	myMeshCreated->LoadMesh(App->modelLoader->meshes[0]);
	newGameObject->ComputeAABB();
	newGameObject->isParentOfMeshes = true;

	allGameObjects.insert(newGameObject);

	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());
	//Deleting model loader information
	App->modelLoader->emptyScene();

	return;
}

void ModuleScene::RemoveGameObject(GameObject * go)
{
	if (!allGameObjects.empty())
	{
		allGameObjects.erase(go);
	}
}

void ModuleScene::SelectObjectInHierarchy(GameObject * selected)
{
	selectedByHierarchy = selected;
}

void ModuleScene::DrawUIBarMenuGameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create House GameObject"))
		{
			GameObject* newGameObject = CreateGameObject();

			newGameObject->myTransform->position += float3(numberOfGameObjects * 4.0f, 0.0f, 0.0f);
			newGameObject->myTransform->UpdateMatrices();

			allGameObjects.insert(newGameObject);


			LoadModel("../Models/baker_house/BakerHouse.fbx", newGameObject);
			
			//-----------------------------------------------------------------//

			GameObject* newGameObjectChild = CreateGameObject("Child", newGameObject);

			newGameObjectChild->myTransform->position += float3(8.0f, 0.0f, 0.0f);
			newGameObjectChild->myTransform->UpdateMatrices();


			LoadModel("../Models/baker_house/BakerHouse.fbx", newGameObjectChild);
			

			allGameObjects.insert(newGameObjectChild);

		}


		ImGui::EndMenu();
	}
}

void ModuleScene::DrawGUI()
{
	unsigned int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiCond_FirstUseEver;
	
	if(showHierarchy)
	{
		ImGui::Begin("Hierarchy", &showHierarchy, flags);
		ImGui::SetWindowSize("Hierarchy",ImVec2(350, 750));
		root->DrawHierarchy(selectedByHierarchy);
		ImGui::End();
	}


	if(selectedByHierarchy != nullptr && showInspector)
	{
		selectedByHierarchy->DrawInspector(showInspector);
	}

}

void ModuleScene::AddToQuadtree(GameObject* go) const
{
	if(go->globalBoundingBox == nullptr)
	{
		LOG("Can not add element to quadtree, AABB is nullptr.");
		return;
	}
	quadtree->Insert(go);
	return;
}

void ModuleScene::RemoveFromQuadTree(GameObject* go) const
{
}

void ModuleScene::BuildQuadTree()
{
	//Recursive
	recursive.StartTimer();
	if (quadTreeInitialized)
		quadtree->Clear();

	for(auto go : allGameObjects)
	{
		if(go->globalBoundingBox != nullptr)
		{
			quadtree->Insert(go);
		}
	}

	quadTreeInitialized = true;
	timeRecursive = recursive.StopTimer();

	iterative.StartTimer();

	//Iterative
	if (quadTreeInitialized)
		quadtreeIterative->ClearIterative();

	for (auto go : allGameObjects)
	{
		if (go->globalBoundingBox != nullptr)
		{
			quadtreeIterative->InsertIterative(quadtreeIterative->nodes,go);
		}
	}

	quadTreeInitialized = true;

	timeIterative = iterative.StopTimer();

	quadtreeIsComputed = true;	
}

void ModuleScene::BuildAABBTree()
{
	aabbTreeTimer.StartTimer();
	for(auto go : allGameObjects)
	{
		if(go->globalBoundingBox != nullptr)
		{
			aabbTree->Insert(go);
		}
	}

	aabbTreeIsComputed = true;

	timeAABBTree = aabbTreeTimer.StopTimer();
}

void ModuleScene::CreateShapesScript()
{
	for(int i = 0; i < 2; ++i)
	{
		CreateGameObjectShape(root, CUBE);
	}
	for (int i = 0; i < 2; ++i)
	{
		CreateGameObjectShape(root, SPHERE);
	}

	for (int i = 0; i < 2; ++i)
	{
		CreateGameObjectShape(root, TORUS);
	}

	for(auto go : allGameObjects)
	{
		if(go != root && go != mainCamera)
		{
			int max = 10;
			int min = -10;
			float3 newPos = float3((float)(std::rand() % (max - min + 1) + min), 0.0f, (float)(rand() % (max - min + 1) + min));
			go->myTransform->TranslateTo(newPos);
		}
	}

	return;
	
}

void ModuleScene::CreateCubesScript()
{
	for (int i = 0; i < 200; ++i)
	{
		CreateGameObjectShape(root, CUBE);
	}

	for (auto go : allGameObjects)
	{
		if (go != root && go != mainCamera)
		{
			int max = 100;
			int min = -100;
			float3 newPos = float3((float)(std::rand() % (max - min + 1) + min), 0.0f, (float)(rand() % (max - min + 1) + min));
			go->myTransform->TranslateTo(newPos);
		}
	}

	return;
}

void ModuleScene::MoveObjects(GameObject* go) const
{
	float dist = 3.0f * sin(App->timemanager->GetRealGameTime());

	
	
	if(go->globalBoundingBox != nullptr)
	{
		if(go->shape == CUBE)
		{
			go->myTransform->position.x += dist;
		}
		if (go->shape == SPHERE)
		{
			go->myTransform->position.y += dist;
		}
		if (go->shape == TORUS)
		{
			go->myTransform->position.z += dist;
		}

	}
	
	return;

}

