#include "Application.h"
#include "ModuleScene.h"
#include "ModuleTimeManager.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleIMGUI.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "MyQuadTree.h"
#include "AABBTree.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "MathGeoLib/Math/float4.h"
#include "Timer.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Geometry/LineSegment.h"
#include "MathGeoLib/Geometry/Plane.h"
#include "debugdraw.h"

#include <random>
#include "SceneLoader.h"
#include <queue>
#include <string>
#include <map>
#include "FontAwesome/IconsFontAwesome5.h"


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
	aabbTree = new AABBTree(10);

	//Creating the main camera of the game
	mainCamera = CreateGameObject("Main Camera", root);
	mainCamera->CreateComponent(CAMERA);

	allGameObjects.insert(mainCamera);
	dynamicGO.insert(mainCamera);
	aabbTree->Insert(mainCamera);

	//Creating the sun light
	directionalLight = CreateGameObject("Directional Light", root);
	directionalLight->CreateComponent(LIGHT);
	directionalLight->myTransform->position = float3(0, 5, 0);

	allGameObjects.insert(directionalLight);
	dynamicGO.insert(directionalLight);
	aabbTree->Insert(directionalLight);

	return true;
}

update_status ModuleScene::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update()
{
	for(auto gameObject : staticGO)
	{
		if(gameObject->isEnabled)
			gameObject->Update();
	}

	for(auto GO : dynamicGO)
	{
		if(GO->isEnabled)
		{
			GO->UpdateTransform();
			GO->Update();

			if (GO->globalBoundingBox != nullptr)
			{
				aabbTree->UpdateObject(GO);
			}

			if (moveObjectsArround)
				MoveObjects(GO);
		}

	}
	//TODO: How to treat cameras : as a normal object but we only put on quadtree objects with mesh or parent of mesh

	DrawGUI();

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	if (quadtreeIsComputed)
	{
		quadtree->ClearIterative();
		delete quadtree;
	}

	quadTreeInitialized = false;

	for(auto GO : allGameObjects)
	{
		delete GO;
	}

	allGameObjects.clear();
	dynamicGO.clear();
	staticGO.clear();

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

	return gameObject;
}

void ModuleScene::LoadModel(const char * path, GameObject* parent)
{
	LOG("Trying to load model in path : %s", path);
	Model modelLoaded;
	App->modelLoader->LoadModel(path, modelLoaded);

	int numObject = 0;
	std::string name = modelLoaded.Name;
	LOG("Creating parent gameObject %s", name.c_str());
	//parent->SetName(name);

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
		dynamicGO.insert(newMeshObject);
		aabbTree->Insert(newMeshObject);
		++numObject;
	}


	parent->ComputeAABB();
	//Setting parent as a meshParent
	parent->isParentOfMeshes = true;

	return;
}

void ModuleScene::CreateEmpty(GameObject* parent)
{
	std::string defaultName = "NewGameObject" + std::to_string(numberOfGameObjects + 1);
	GameObject* empty = CreateGameObject(defaultName.c_str(), parent);
	

	allGameObjects.insert(empty);
	dynamicGO.insert(empty);
	aabbTree->Insert(empty);

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
	std::string defaultName = "ParentBakerHouse" + std::to_string(numberOfBakerHouse + 1);
	GameObject* newGameObject = CreateGameObject(defaultName.c_str(), parent);
	LoadModel("BakerHouse", newGameObject);
	++numberOfBakerHouse;

	allGameObjects.insert(newGameObject);
	dynamicGO.insert(newGameObject);
	aabbTree->Insert(newGameObject);
	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());
	

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
	LoadModel("Zombunny", newGameObject);
	++numberOfBakerHouse;

	allGameObjects.insert(newGameObject);
	dynamicGO.insert(newGameObject);
	aabbTree->Insert(newGameObject);
	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());


	return;
}

void ModuleScene::CreateGameObjectByName(GameObject * parent, const char* name)
{
	if (parent == nullptr)
	{
		LOG("ERROR: Parent is nullptr, cannot create gameObject.");
		return;
	}

	LOG("Creating a %s.", name);
	GameObject* newGameObject = CreateGameObject(name, parent);
	LoadModel(name, newGameObject);
	++numberOfBakerHouse;

	allGameObjects.insert(newGameObject);
	dynamicGO.insert(newGameObject);
	aabbTree->Insert(newGameObject);
	LOG("%s created with %s as parent.", name, parent->GetName());


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
		
		if (go->isStatic)
		{
			staticGO.erase(go);
		}
		else
		{
			dynamicGO.erase(go);
			aabbTree->Remove(go);
		}
			
	}

	return;
}

void ModuleScene::SelectObjectInHierarchy(GameObject * selected)
{
	selectedByHierarchy = selected;

	return;
}


void ModuleScene::DrawGUI()
{
	unsigned int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiCond_FirstUseEver;
	
	if(showHierarchy)
	{
		ImGui::SetNextWindowPos(
			ImVec2(0, 18)

		);
		ImGui::SetNextWindowSize(
			ImVec2(App->window->width * App->imgui->hierarchySizeRatioWidth, App->window->height * App->imgui->hierarchySizeRatioHeight)
		);

		ImGui::Begin(ICON_FA_SITEMAP " Hierarchy", &showHierarchy, flags);
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

void ModuleScene::BuildQuadTree()
{
	if (staticGO.size() == 0 || (staticGO.size() == 1 && (*staticGO.begin())->globalBoundingBox == nullptr))
		return;

	AABB* sceneBox = ComputeSceneAABB();

	quadtreeIsComputed = false;

	//Iterative
	if (quadTreeInitialized)
	{
		quadtree->ClearIterative();
		delete quadtree;
	}
	
	quadtree = new MyQuadTree(sceneBox);

	iterative.StartTimer();
	for(auto go : staticGO)
	{
		if(go->globalBoundingBox != nullptr)
		{
			quadtree->InsertIterative(quadtree->nodes, go);
		}
	}

	timeIterative = iterative.StopTimer();

	quadTreeInitialized = true;

	quadtreeIsComputed = true;	

	return;
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

void ModuleScene::CreateHousesScript()
{
	for (int i = 0; i < 1000; ++i)
	{
		CreateGameObjectBakerHouse(root);
	}

	for (auto go : allGameObjects)
	{
		if (go != root && go != mainCamera && go->isParentOfMeshes)
		{
			int max = 100;
			int min = -100;
			float3 newPos = float3((float)(std::rand() % (max - min + 1) + min), 0.0f, (float)(rand() % (max - min + 1) + min));
			go->myTransform->TranslateTo(newPos);
		}
	}

	return;
}

AABB * ModuleScene::ComputeSceneAABB() const
{
	auto someElementIterator = staticGO.begin();
	while((*someElementIterator)->globalBoundingBox == nullptr)
	{
		++someElementIterator;
	}
	float3 minPoint = (*someElementIterator)->globalBoundingBox->minPoint;
	float3 maxPoint = (*someElementIterator)->globalBoundingBox->maxPoint;
	
	for(auto it = ++someElementIterator; it != staticGO.end(); ++it)
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
	for (int i = 0; i < 50; ++i)
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
	staticGO.clear();
	dynamicGO.clear();

	//Create root
	root = new GameObject();
	root->OnLoad(*loader);

	selectedByHierarchy = root;

	//Create AABBtree
	aabbTree = new AABBTree(10);

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
		//Add to quadtree or aabbtree set (dynamic or static)
		if (currentGameObject->isStatic)
			staticGO.insert(currentGameObject);
		else
		{
			dynamicGO.insert(currentGameObject);
			if(currentGameObject->globalBoundingBox != nullptr)
				aabbTree->Insert(currentGameObject);
		}

		//Add gameobject to queue
		parents.push(currentGameObject);
	}

	//Build QuadTree
	BuildQuadTree();

	delete loader;

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
	++go->numberOfCopies;

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

LineSegment* ModuleScene::CreateRayCast(float3 origin, float3 direction, float maxDistance)
{
	Frustum auxFrustum = Frustum();
	auxFrustum.pos = origin - float3(0.1f,0.1f,0.1f);
	auxFrustum.type = FrustumType::PerspectiveFrustum;
	auxFrustum.nearPlaneDistance = 0.1f;
	auxFrustum.farPlaneDistance = maxDistance;
	auxFrustum.front = direction;
	auxFrustum.up = (direction.Cross(float3(0,1,0))).Cross(direction);
	auxFrustum.verticalFov = (float)M_PI / 4.0f;
	auxFrustum.horizontalFov = 2.0f * atanf(tanf(auxFrustum.verticalFov * 0.5f) *1.0f);

	Plane nearPlane = auxFrustum.NearPlane();
	nearPlane.ClosestPoint(origin);

	float normalized_x = 0.0f;
	float normalized_y = 0.0f;

	return new LineSegment(auxFrustum.UnProjectLineSegment(normalized_x, normalized_y));
}

LineSegment* ModuleScene::CreateRayCast(float normalizedX, float normalizedY) const	
{
	return new LineSegment(App->camera->editorCamera->frustum->UnProjectLineSegment(normalizedX, normalizedY));
}

void ModuleScene::PickObject(const ImVec2 &sizeWindow, const ImVec2 &posWindow)
{
	float2 mouse(App->input->GetMousePosition().x, App->input->GetMousePosition().y);
	//Move offset for cursor being on top of mouse
	mouse += offset;
	float normalizedX, normalizedY;
	//Start is position of scene imgui window and stop is scene imgui window + width/heigth of scene imgui window size
	normalizedX = mapValues(mouse.x, posWindow.x, posWindow.x + sizeWindow.x, -1, 1);
	normalizedY = mapValues(mouse.y, posWindow.y, posWindow.y + sizeWindow.y, 1, -1);
	LineSegment ray = *CreateRayCast(normalizedX, normalizedY);
	GameObject* selectedGO = IntersectRayCast(App->camera->editorCamera->frustum->pos, ray);
	if (selectedGO != nullptr)
	{
		if (!selectedGO->isParentOfMeshes && selectedGO->parent != nullptr)
		{
			selectedGO = selectedGO->parent;
		}
		selectedByHierarchy = selectedGO;
	}

	dd::arrow(ray.a, ray.b, float3(1, 0, 0),10);

	return;
}

GameObject * ModuleScene::GetRoot() const
{
	return root;
}

GameObject* ModuleScene::IntersectRayCast(float3 origin, const LineSegment &ray)
{
	//First get aabb intersection ordered by distance, then compare with aabb that are closer to the closest triangle hit
	std::map<float, GameObject*> hits;

	for(auto go : allGameObjects)
	{
		if(go->globalBoundingBox != nullptr)
		{
			bool hit = ray.Intersects(*go->globalBoundingBox);
			if(hit)
			{
				float dist = origin.Distance(*go->globalBoundingBox);
				hits[dist] = go;
			}

		}
	
	}
	
	if (hits.size() == 0)
		return nullptr;

	float minDistTriangle = hits.rbegin()->first + 1.0f;
	GameObject* hitGO = nullptr;
	bool isHit = false;
	
	//Until first hit with triangle check all gameObjects by order
	for(const auto& it : hits)
	{
		if(it.first < minDistTriangle)
		{
			//If hitDist is -1.0f then ray doesn't intersect any triangle
			float hitDist = it.second->IsIntersectedByRay(origin, ray);
			if (hitDist != -1.0f)
			{
				isHit = true;
				minDistTriangle = hitDist;
				hitGO = it.second;
			}
			
		}

	}

	return hitGO;
}



