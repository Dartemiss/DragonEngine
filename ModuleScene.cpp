#include "ModuleScene.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "include/Math/float4.h"


ModuleScene::ModuleScene()
{
	root = new GameObject("World");
	root->isRoot = true;
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	//Creating the main camera of the game
	mainCamera = CreateGameObject("Main Camera", root);
	mainCamera->CreateComponent(CAMERA);


	allGameObjects.push_back(mainCamera);

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

	return gameObject;
}

GameObject * ModuleScene::CreateGameObject(const char * name, GameObject * parent)
{
	GameObject* gameObject = new GameObject(name);
	gameObject->SetParent(parent);

	LOG("Creating new GameObject with name: %s", name);

	++numberOfGameObjects;
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
		allGameObjects.push_back(newMeshObject);

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
	

	allGameObjects.push_back(empty);

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

	allGameObjects.push_back(newGameObject);
	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());
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
	

	if(!App->modelLoader->meshes.size() == 1)
	{
		LOG("ERROR: Sphere model cannot have more than one mesh. ");
		delete newGameObject;
		return;
	}



	ComponentMesh* myMeshCreated = (ComponentMesh*)newGameObject->CreateComponent(MESH);
	myMeshCreated->LoadMesh(App->modelLoader->meshes[0]);
	newGameObject->ComputeAABB();
	allGameObjects.push_back(newGameObject);

	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());
	//Deleting model loader information
	App->modelLoader->emptyScene();
}

void ModuleScene::RemoveGameObject(GameObject * go)
{
	if (!allGameObjects.empty())
	{
		allGameObjects.erase(std::remove(allGameObjects.begin(), allGameObjects.end(), go), allGameObjects.end());
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

			allGameObjects.push_back(newGameObject);


			LoadModel("../Models/baker_house/BakerHouse.fbx", newGameObject);
			
			//-----------------------------------------------------------------//

			GameObject* newGameObjectChild = CreateGameObject("Child", newGameObject);

			newGameObjectChild->myTransform->position += float3(8.0f, 0.0f, 0.0f);
			newGameObjectChild->myTransform->UpdateMatrices();


			LoadModel("../Models/baker_house/BakerHouse.fbx", newGameObjectChild);
			

			allGameObjects.push_back(newGameObjectChild);

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

