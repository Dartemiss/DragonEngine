#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"


ModuleScene::ModuleScene()
{
	root = new GameObject("World");
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
			newGameObject->CreateComponent(MESH);
			newGameObject->CreateComponent(MATERIAL);

			newGameObject->myTransform->position += float3(numberOfGameObjects * 4.0f, 0.0f, 0.0f);
			newGameObject->myTransform->UpdateMatrices();

			if(newGameObject->myMeshes != nullptr)
			{
				newGameObject->LoadModel("../Models/baker_house/BakerHouse.fbx");
			}

			allGameObjects.push_back(newGameObject);


			GameObject* newGameObjectChild = CreateGameObject("Child", newGameObject);
			newGameObjectChild->CreateComponent(MESH);
			newGameObjectChild->CreateComponent(MATERIAL);

			newGameObjectChild->myTransform->position += float3(8.0f, 0.0f, 0.0f);
			newGameObjectChild->myTransform->UpdateMatrices();

			if (newGameObjectChild->myMeshes != nullptr)
			{
				newGameObjectChild->LoadModel("../Models/baker_house/BakerHouse.fbx");
			}

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
		ImGui::Begin("Inspector", &showInspector);
		if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Position");
			ImGui::DragFloat3("Position", (float *)&selectedByHierarchy->myTransform->position, 0.1f);
			ImGui::Text("Rotation");
			ImGui::DragFloat3("Rotation", (float *)&selectedByHierarchy->myTransform->eulerRotation, 1.0f, -360.0f, 360.0f);
			ImGui::Text("Scale");
			ImGui::DragFloat3("Scale", (float *)&selectedByHierarchy->myTransform->scale, 0.01f, 0.01f, 1000.0f);
			
		}
		ImGui::End();
		//Change EulerRotation to Quat
		selectedByHierarchy->myTransform->EulerToQuat();
	}

}

