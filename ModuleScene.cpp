#include "ModuleScene.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"


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

	return;
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


			LoadModel("../Models/baker_house/BakerHouse.fbx",newGameObject);
			
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
