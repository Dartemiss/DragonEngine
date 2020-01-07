#include "ModuleScene.h"
#include "ModuleTimeManager.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "Timer.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "MyQuadTree.h"
#include "AABBTree.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "MathGeoLib/Math/float4.h"
#include <random>

#include "SceneLoader.h"
#include <queue>
#include <string>

using namespace std;

ModuleScene::ModuleScene()
{
	root = new GameObject("World");
	root->UID = 1;
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

	//quadtree = new MyQuadTree(AABB(float3(-100,0,-40), float3(100,0,100)),1);
	//quadtreeIterative = new MyQuadTree(new AABB(float3(-100, 0, -100), float3(100, 0, 100)));
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
	if (quadtreeIsComputed)
	{
		quadtree->Clear();
		quadtreeIterative->ClearIterative();
		delete quadtree;
		delete quadtreeIterative;
	}

	for(auto GO : allGameObjects)
	{
		delete GO;
	}

	delete aabbTree;
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

GameObject * ModuleScene::CreateGameObject(GameObject * go)
{
	GameObject* gameObject = new GameObject(*go);

	return nullptr;
}

void ModuleScene::LoadModel(const char * path, GameObject* parent)
{
	LOG("Trying to load model in path : %s", path);
	Model modelLoaded;
	App->modelLoader->LoadModel(path, modelLoaded);

	int numObject = 0;
	std::string name = modelLoaded.Name;
	LOG("Creating parent gameObject %s", name.c_str());
	parent->SetName(name);

	LOG("For each mesh of the model we create a gameObject.");
	
	for (multimap<Mesh*, Texture*>::iterator it = modelLoaded.Meshes.begin(); it != modelLoaded.Meshes.end(); ++it)
	{
		std::string newName = name + std::to_string(numObject);
		GameObject* newMeshObject = CreateGameObject(newName.c_str(), parent);
		ComponentMesh* myMeshCreated = (ComponentMesh*)newMeshObject->CreateComponent(MESH);
		ComponentMaterial* myMaterialCreated = (ComponentMaterial*)newMeshObject->CreateComponent(MATERIAL);


		myMeshCreated->LoadMesh(it->first);
		vector<Texture*> textures;
		textures.push_back(it->second);
		myMaterialCreated->SetTextures(textures);
		newMeshObject->ComputeAABB();
		allGameObjects.insert(newMeshObject);

		++numObject;
	}



	/*for(auto mesh : App->modelLoader->meshes)
	{
		std::string newName = name + std::to_string(numObject);
		GameObject* newMeshObject = CreateGameObject(newName.c_str(), parent);
		ComponentMesh* myMeshCreated = (ComponentMesh*)newMeshObject->CreateComponent(MESH);
		ComponentMaterial* myMaterialCreated = (ComponentMaterial*)newMeshObject->CreateComponent(MATERIAL);
		

		myMeshCreated->LoadMesh(mesh);
		myMaterialCreated->SetTextures(myMeshCreated->mesh->textures);
		newMeshObject->ComputeAABB();
		allGameObjects.insert(newMeshObject);

		++numObject;
	}*/

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
	LoadModel("BakerHouse", newGameObject);
	++numberOfBakerHouse;

	allGameObjects.insert(newGameObject);
	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());

	if(quadTreeInitialized)
		AddToQuadtree(newGameObject);

	return;
}

void ModuleScene::CreateGameObjectZomBunny(GameObject * parent)
{
	if (parent == nullptr)
	{
		LOG("ERROR: Parent is nullptr, cannot create gameObject.");
		return;
	}

	LOG("Creating a Zom Bunny.");
	std::string defaultName = "ZomBunny";
	GameObject* newGameObject = CreateGameObject(defaultName.c_str(), parent);
	LoadModel("../Models/ZomBunny/Zombunny.fbx", newGameObject);
	++numberOfBakerHouse;

	allGameObjects.insert(newGameObject);
	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());

	if (quadTreeInitialized)
		AddToQuadtree(newGameObject);

	return;
}

void ModuleScene::CreateGameObjectShape(GameObject * parent, ShapeType shape)
{
	//if (parent == nullptr)
	//{
	//	LOG("ERROR: Parent is nullptr, cannot create gameObject.");
	//	return;
	//}

	//std::string defaultName;
	//bool correct;
	//switch (shape)
	//{
	//case SPHERE:
	//	LOG("Creating a GameObject with Sphere Mesh.");
	//	defaultName = "Sphere" + std::to_string(numberOfSphere + 1);
	//	correct = App->modelLoader->LoadSphere(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(0.4f, 0.4f, 0.4f, 0.4f));
	//	if (!correct)
	//	{
	//		LOG("ERROR: Cannot load the sphere mesh correctly.");
	//		return;
	//	}
	//	++numberOfSphere;
	//	break;
	//case CUBE:
	//	LOG("Creating a GameObject with cube Mesh.");
	//	defaultName = "Cube" + std::to_string(numberOfCube + 1);
	//	correct = App->modelLoader->LoadCube("cube0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, float4(0.4f, 0.4f, 0.4f, 0.4f));
	//	if (!correct)
	//	{
	//		LOG("ERROR: Cannot load the cube mesh correctly.");
	//		return;
	//	}
	//	++numberOfCube;
	//	break;
	//case CYLINDER:
	//	LOG("Creating a GameObject with Cylinder Mesh.");
	//	defaultName = "Cylinder" + std::to_string(numberOfCylinder + 1);
	//	correct = App->modelLoader->LoadCylinder(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(0.4f, 0.4f, 0.4f, 0.4f));
	//	if (!correct)
	//	{
	//		LOG("ERROR: Cannot load the cylinder mesh correctly.");
	//		return;
	//	}
	//	++numberOfCylinder;
	//	break;
	//case TORUS:
	//	LOG("Creating a GameObject with torus Mesh.");
	//	defaultName = "Torus" + std::to_string(numberOfTorus + 1);
	//	correct = App->modelLoader->LoadTorus(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 0.5f, 0.67f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
	//	if (!correct)
	//	{
	//		LOG("ERROR: Cannot load the torus mesh correctly.");
	//		return;
	//	}
	//	++numberOfTorus;
	//	break;
	//default:
	//	break;
	//}


	//GameObject* newGameObject = CreateGameObject(defaultName.c_str(), parent);
	//
	//newGameObject->shape = shape;

	//if(!App->modelLoader->meshes.size() == 1)
	//{
	//	LOG("ERROR: Sphere model cannot have more than one mesh. ");
	//	delete newGameObject;
	//	return;
	//}



	//ComponentMesh* myMeshCreated = (ComponentMesh*)newGameObject->CreateComponent(MESH);
	//myMeshCreated->LoadMesh(App->modelLoader->meshes[0]);
	//ComponentMaterial* myMaterialCreated = (ComponentMaterial*)newGameObject->CreateComponent(MATERIAL);
	//myMaterialCreated->SetTextures(myMeshCreated->mesh->textures);
	//newGameObject->ComputeAABB();
	//newGameObject->isParentOfMeshes = true;

	//allGameObjects.insert(newGameObject);


	//LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());
	////Deleting model loader information
	//App->modelLoader->emptyScene();

	return;
}

void ModuleScene::RemoveGameObject(GameObject * go)
{
	if (!allGameObjects.empty())
	{
		allGameObjects.erase(go);
	}

	return;
}

void ModuleScene::SelectObjectInHierarchy(GameObject * selected)
{
	selectedByHierarchy = selected;

	return;
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
	AABB* sceneBox = ComputeSceneAABB();

	quadtreeIsComputed = false;

	//Recursive
	if (quadTreeInitialized)
	{
		quadtree->Clear();
		delete quadtree;
	}
	
	quadtree = new MyQuadTree(*sceneBox,1);

	recursive.StartTimer();
	for(auto go : allGameObjects)
	{
		if(go->globalBoundingBox != nullptr)
		{
			quadtree->Insert(go);
		}
	}

	timeRecursive = recursive.StopTimer();

	

	//Iterative
	if (quadTreeInitialized)
	{
		quadtreeIterative->ClearIterative();
		delete quadtreeIterative;
	}
		
	quadtreeIterative = new MyQuadTree(sceneBox);

	iterative.StartTimer();
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

AABB * ModuleScene::ComputeSceneAABB() const
{
	auto someElementIterator = allGameObjects.begin();
	while((*someElementIterator)->globalBoundingBox == nullptr)
	{
		++someElementIterator;
	}
	float3 minPoint = (*someElementIterator)->globalBoundingBox->minPoint;
	float3 maxPoint = (*someElementIterator)->globalBoundingBox->maxPoint;
	
	for(auto it = ++someElementIterator; it != allGameObjects.end(); ++it)
	{
		if((*it)->globalBoundingBox != nullptr)
		{
			minPoint = Min(minPoint, (*it)->globalBoundingBox->minPoint);
			maxPoint = Max(maxPoint, (*it)->globalBoundingBox->maxPoint);
		}

	}

	minPoint.y = 0;
	maxPoint.y = 0;

	return new AABB(minPoint - float3(5,0,5), maxPoint + float3(5, 0, 5));
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

void ModuleScene::SaveScene()
{
	SceneLoader * loader = new SceneLoader();

	root->OnSave(*loader);
	for (set<GameObject*>::iterator it = allGameObjects.begin(); it != allGameObjects.end(); ++it)
		(*it)->OnSave(*loader);

	loader->SaveJSONToFile("temp_save.json");

	delete loader;
}

void ModuleScene::LoadScene()
{
	SceneLoader * loader = new SceneLoader();

	loader->LoadJSONFromFile("temp_save.json");

	//Check root node exists
	if (!loader->SetCurrentObject(0))
	{
		LOG("Root node does not exist! Cant load Scene.");
		return;
	}

	//Remove previous data
	CleanUp();
	allGameObjects.clear();

	//Create root
	root = new GameObject();
	root->OnLoad(*loader);

	//Start queue for loading the rest of the scene
	queue<GameObject*> parents;
	parents.push(root);
	GameObject * parent;
	GameObject * currentGameObject;

	while (parents.size() > 0)
	{
		//Search for gameobject with same parent uid
		parent = parents.front();
		if (!loader->SetCurrentObject(parent->UID))
		{
			//If no gameobject is found, go to next parent
			parents.pop();
			continue;
		}
		
		//Create gameobject and link parent
		currentGameObject = new GameObject();
		currentGameObject->OnLoad(*loader);
		currentGameObject->SetParent(parent);

		if (currentGameObject->GetName() == "Main Camera")
			mainCamera = currentGameObject;

		allGameObjects.insert(currentGameObject);

		//Add gameobject to queue
		parents.push(currentGameObject);
	}
}

void ModuleScene::PasteGameObject(GameObject * go)
{
	assert(go != nullptr);
	if(clipboard == nullptr)
	{
		LOG("You have nothing copied on the clipboard.");
		return;
	}
	GameObject* pastedGO = new GameObject(*clipboard, go);
	go->children.push_back(pastedGO);
	++clipboard->numberOfCopies;

	allGameObjects.insert(pastedGO);
	//Add all childs to the scene

	InsertChilds(pastedGO);

	return;
}

void ModuleScene::DuplicateGameObject(GameObject * go)
{
	assert(go != nullptr);

	if(go->UID == 1)
	{
		LOG("ERROR: You cannot duplicate the root. STOP!");
		return;
	}

	GameObject* duplicatedGO = new GameObject(*go, go->parent);
	go->parent->children.push_back(duplicatedGO);
	++clipboard->numberOfCopies;

	allGameObjects.insert(duplicatedGO);
	//Add all childs to the scene

	InsertChilds(duplicatedGO);

	return;
}


void ModuleScene::InsertChilds(GameObject * go)
{
	assert(go != nullptr);

	for(auto ch : go->children)
	{
		allGameObjects.insert(ch);
		InsertChilds(ch);
	}


	return;
}

