#include "GameObject.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleIMGUI.h"
#include "ModuleWindow.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"
#include "AABBTree.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "SDL/SDL.h"
#include "Imgui/imgui_stdlib.h"
#include "MathGeoLib/Geometry/LineSegment.h"
#include "debugdraw.h"
#include "UUIDGenerator.h"
#include "SceneLoader.h"
#include "FontAwesome/IconsFontAwesome5.h"
#include <set>

using namespace std;

GameObject::GameObject()
{
}

GameObject::GameObject(const char * name)
{
	this->name = name;
	CreateComponent(TRANSFORM);
	bool validUID = false;
	//Ensure that UID is unique
	while(!validUID && name != "World")
	{
		this->UID = UUIDGen->getUUID();
		if(App->scene->UIDs.find(UID) == App->scene->UIDs.end())
		{
			App->scene->UIDs.insert(UID);
			validUID = true;
		}
	}
	boundingBox = new AABB(myTransform->position - float3(1, 1, 1), myTransform->position + float3(1, 1, 1));
	globalBoundingBox = new AABB(*boundingBox);

}

GameObject::GameObject(const GameObject &go, GameObject* parent)
{
	this->name = go.name + std::to_string(go.numberOfCopies);

	shape = go.shape;
	boundingBox = new AABB(*go.boundingBox);
	globalBoundingBox = new AABB(*go.globalBoundingBox);

	for(auto comp : go.components)
	{
		Component* aux;
		switch (comp->myType)
		{
			case TRANSFORM:
				 aux = new ComponentTransform(this, (ComponentTransform*)comp);
				break;
			case MESH:
				aux = new ComponentMesh(this, (ComponentMesh*)comp);
				break;
			case MATERIAL:
				aux = new ComponentMaterial(this, (ComponentMaterial*)comp);
				break;
			case CAMERA:
				aux = new ComponentCamera(this, (ComponentCamera*)comp);
				break;
			default:
				break;
		}

		components.push_back(aux);
	}

	for(auto myComp : components)
	{
		if (myComp->myType == TRANSFORM)
			myTransform = (ComponentTransform*)myComp;

		if (myComp->myType == MESH)
			myMesh = (ComponentMesh*)myComp;

		if (myComp->myType == MATERIAL)
			myMaterial = (ComponentMaterial*)myComp;

	}

	//Get a copy of all childs
	for(auto child : go.children)
	{
		GameObject* newChild = new GameObject(*child, this);
		children.push_back(newChild);
	}

	this->parent = parent;
	//UID substitute
	
	isStatic = go.isStatic;
	isParentOfMeshes = go.isParentOfMeshes;

	this->UID = UUIDGen->getUUID();

}


GameObject::~GameObject()
{
	for (auto comp : components)
	{
		comp->CleanUp();
		delete comp;
	}

	delete boundingBox;
	delete globalBoundingBox;

}

void GameObject::Update()
{
	for(auto component : components)
	{
		if (component->myType != TRANSFORM)
			component->Update();
	}

	return;
}

void GameObject::SetParent(GameObject * newParent)

{
	//Erase me from previous father
	if (parent != nullptr)
	{
		parent->RemoveChildren(this);
	}
		
	
	if(newParent != nullptr)
	{
		LOG("Setting new GamesObject parent and children.")
		parent = newParent;
		parent->children.push_back(this);

		if(myMesh != nullptr)
			parent->isParentOfMeshes = true;

		return;
	}



	LOG("ERROR: Cannot set parent because new Parent is nullptr.");
	return;

}

void GameObject::RemoveChildren(GameObject * child)
{
	//TODO: crash when deleting player
	if(!children.empty())
	{
		children.erase(std::find(children.begin(), children.end(), child));
	}
	
	return;
}

void GameObject::DeleteGameObject()
{
	if (UID == 1)
	{
		LOG("Cannot delete root STOP!");
		return;
	}

	parent->RemoveChildren(this);
	App->scene->RemoveGameObject(this);
	for(auto ch : children)
	{
		ch->DeleteGameObject();
	}

	if (App->scene->selectedByHierarchy == this)
		App->scene->selectedByHierarchy = nullptr;

	CleanUp();

	
}

void GameObject::CleanUp()
{
	delete this;
}

Component * GameObject::CreateComponent(ComponentType type)
{
	Component* component = nullptr;

	switch(type)
	{
		case TRANSFORM:
			component = new ComponentTransform(this);
			myTransform = (ComponentTransform*)component;
			break;
		case MESH:
			component = new ComponentMesh(this);
			myMesh = (ComponentMesh*)component;
			break;
		case MATERIAL:
			component = new ComponentMaterial(this);
			myMaterial = (ComponentMaterial*)component;
			break;
		case CAMERA:
			component = new ComponentCamera(this);
			break;
		case LIGHT:
			component = new ComponentLight(this);
			myLight = (ComponentLight*)component;
			break;
		default:
			LOG("ERROR: INVALID TYPE OF COMPONENT");
			return nullptr;
			break;
	}

	component->myGameObject = this;

	components.push_back(component);

	return component;
}


void GameObject::DrawHierarchy(GameObject * selected)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen
		| ImGuiTreeNodeFlags_OpenOnDoubleClick | (selected == this ? ImGuiTreeNodeFlags_Selected : 0);

	ImGui::PushID(this);
	if (children.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool objOpen = ImGui::TreeNodeEx(this, flags, name.c_str());

	if(ImGui::IsItemClicked())
	{
		App->scene->SelectObjectInHierarchy(this);
	}

	if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && ImGui::IsItemHovered()) // Could be also || ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("Creation Popup");
	}

	if (ImGui::BeginPopup("Creation Popup"))
	{
		
		if (ImGui::Selectable("Copy"))
		{
			if (this->UID != 1)
				App->scene->clipboard = this;
			else
				LOG("Root cannot be copied. STOP!");
		}
		if (ImGui::Selectable("Paste"))
		{
			App->scene->PasteGameObject(this);
		}
		
		ImGui::Separator();

		if (ImGui::Selectable("Rename"))
		{
			//TODO: Rename gameobjects
		}
		if (ImGui::Selectable("Duplicate"))
		{
			App->scene->DuplicateGameObject(this);
		}

		if (ImGui::Selectable("Delete"))
		{
			DeleteGameObject();
		}

		ImGui::Separator();

		if(ImGui::Selectable("Create Empty"))
		{
			//Create empty gameobject
			App->scene->CreateEmpty(this);
		}

		if (ImGui::BeginMenu("Create 3D Object"))
		{
			/*
			if(ImGui::MenuItem("Cube"))
			{
				App->scene->CreateGameObjectShape(this, CUBE);
			}

			if (ImGui::MenuItem("Sphere"))
			{
				App->scene->CreateGameObjectShape(this, SPHERE);
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				App->scene->CreateGameObjectShape(this, CYLINDER);
			}

			if (ImGui::MenuItem("Torus"))
			{
				App->scene->CreateGameObjectShape(this, TORUS);
			}
			*/
			if (ImGui::MenuItem("Baker House"))
			{
				App->scene->CreateGameObjectBakerHouse(this);
			}
			if (ImGui::MenuItem("ZomBunny"))
			{
				App->scene->CreateGameObjectByName(this, "ZomBunny");
			}
			if (ImGui::MenuItem("Player"))
			{
				App->scene->CreateGameObjectByName(this, "Player");
			}

			if (ImGui::MenuItem("Drawers"))
			{
				App->scene->CreateGameObjectByName(this, "Drawers");
			}
			if (ImGui::MenuItem("Clock"))
			{
				App->scene->CreateGameObjectByName(this, "Clock");
			}
			if (ImGui::MenuItem("Wall"))
			{
				App->scene->CreateGameObjectByName(this, "Wall");
			}
			if (ImGui::MenuItem("Dollhouse"))
			{
				App->scene->CreateGameObjectByName(this, "Dollhouse");
			}
			if (ImGui::MenuItem("Floor"))
			{
				App->scene->CreateGameObjectByName(this, "Floor");
			}
			if (ImGui::MenuItem("Hearse"))
			{
				App->scene->CreateGameObjectByName(this, "Hearse");
			}
			if (ImGui::MenuItem("Robot"))
			{
				App->scene->CreateGameObjectByName(this, "Robot");
			}

			if (ImGui::MenuItem("Firetruck"))
			{
				App->scene->CreateGameObjectByName(this, "Firetruck");
			}

			if (ImGui::MenuItem("SpinningTop"))
			{
				App->scene->CreateGameObjectByName(this, "SpinningTop");
			}
			if (ImGui::MenuItem("Stool"))
			{
				App->scene->CreateGameObjectByName(this, "Stool");
			}
			


			ImGui::EndMenu();
		}

			
		ImGui::EndPopup();
	}


	CheckDragAndDrop(this);

	if(objOpen)
	{
		for(auto child : children)
		{
			child->DrawHierarchy(selected);
		}
		if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			ImGui::TreePop();
		}
	}
	ImGui::PopID();
}

void GameObject::DrawCamera()
{
	for(auto comp : components)
	{
		if(comp->myType == CAMERA)
		{
			((ComponentCamera*)comp)->DrawFrustum();
		}
	}
}

void GameObject::UpdateTransform()
{
	if(myTransform != nullptr)
	{
		if(parent != nullptr && parent->myTransform != nullptr)
		{
			myTransform->SetGlobalMatrix(parent->myTransform->globalModelMatrix);
		}
		myTransform->UpdateMatrices();

		if(globalBoundingBox != nullptr && boundingBox != nullptr)
		{
			//AABB Global Update
			//Compute globalBoundingBox

			AABB auxBox;
			auxBox.SetNegativeInfinity();
			auxBox.Enclose(*boundingBox);
			auxBox.TransformAsAABB(myTransform->globalModelMatrix);

			*globalBoundingBox = auxBox;
			
			
		}
	}
}

void GameObject::SetName(const std::string &newName)
{
	name = newName;
}

std::string GameObject::GetName() const
{
	return name;
}

void GameObject::ComputeAABB()
{
	float3 min = float3(-1,-1,-1);
	float3 max = float3(1,1,1);

	if(myMesh == nullptr)
	{
		LOG("This gameObject does not have a Mesh thus we compute the AABB from his childs.");

		if(children.size() == 0)
		{
			LOG("Cannot compute the AABB because gameObject does not have children.");
			return;
		}

		for(auto child : children)
		{
			if(child->boundingBox != nullptr)
			{
				//Min vertex
				if (child->boundingBox->minPoint.x < min.x)
					min.x = child->boundingBox->minPoint.x;
				if (child->boundingBox->minPoint.y < min.y)
					min.y = child->boundingBox->minPoint.y;
				if (child->boundingBox->minPoint.z < min.z)
					min.z = child->boundingBox->minPoint.z;
				//Max vertex
				if (child->boundingBox->maxPoint.x > max.x)
					max.x = child->boundingBox->maxPoint.x;
				if (child->boundingBox->maxPoint.y > max.y)
					max.y = child->boundingBox->maxPoint.y;
				if (child->boundingBox->maxPoint.z > max.z)
					max.z = child->boundingBox->maxPoint.z;
			}
		}

		boundingBox = new AABB(min, max);
		//Compute globalBoundingBox
		float3 globalPos, globalScale;
		float3x3 globalRot;
		myTransform->globalModelMatrix.Decompose(globalPos, globalRot, globalScale);

		globalBoundingBox = new AABB(min.Mul(globalScale) + globalPos, max.Mul(globalScale) + globalPos);

		return;
	}
		

	for (auto vertex : myMesh->mesh->vertices)
	{
		//Min vertex
		if (vertex.Position.x < min.x)
			min.x = vertex.Position.x;
		if (vertex.Position.y < min.y)
			min.y = vertex.Position.y;
		if (vertex.Position.z < min.z)
			min.z = vertex.Position.z;
		//Max vertex
		if (vertex.Position.x > max.x)
			max.x = vertex.Position.x;
		if (vertex.Position.y > max.y)
			max.y = vertex.Position.y;
		if (vertex.Position.z > max.z)
			max.z = vertex.Position.z;
	}
	
	boundingBox = new AABB(min, max);

	//Compute globalBoundingBox
	float3 globalPos, globalScale;
	float3x3 globalRot;
	myTransform->globalModelMatrix.Decompose(globalPos, globalRot, globalScale);

	globalBoundingBox = new AABB(min.Mul(globalScale) + globalPos, max.Mul(globalScale) + globalPos);

	return;
}

void GameObject::DrawAABB() const
{
	dd::aabb(globalBoundingBox->minPoint, globalBoundingBox->maxPoint, float3(0, 1, 0));
}

void GameObject::Draw(const unsigned int program, bool isGamePlaying, bool drawAABB)
{
	if (myLight != nullptr)
	{
		myLight->SetDrawLightsForMeshes(program);
		if (!isGamePlaying)
			myLight->Draw();
	}

	if (!isGamePlaying && isParentOfMeshes && boundingBox != nullptr && drawAABB)
	{
		DrawAABB();
	}

	if(myMesh != nullptr)
	{
		myMaterial->SetDrawTextures(program);
		myMesh->Draw(program);
	}
}

void GameObject::DrawInspector(bool &showInspector)
{
	ImGui::SetNextWindowPos(
		ImVec2(App->window->width * App->imgui->inspectorPosRatioWidth, App->window->height * App->imgui->inspectorPosRatioHeight)
	);
	ImGui::SetNextWindowSize(
		ImVec2(App->window->width * App->imgui->inspectorSizeRatioWidth, App->window->height * App->imgui->inspectorSizeRatioHeight)
	);

	ImGui::Begin(ICON_FA_INFO_CIRCLE " Inspector", &showInspector);

	ImGui::Checkbox("", &isEnabled); ImGui::SameLine();
	
	ImGui::InputText("##Name", &name);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0, 1, 0, 1));

	if(ImGui::Checkbox("Static", &isStatic))
	{
		SetStatic();
	}

	ImGui::PopStyleColor();

	//Draw Components
	for(auto c : components)
	{
		c->DrawInspector();
	}

	ImGui::End();
	//Change EulerRotation to Quat
	myTransform->EulerToQuat();
}

void GameObject::OnSave(SceneLoader & loader)
{
	loader.StartGameObject();
	loader.AddUnsignedInt("UID", UID);
	if (parent != nullptr)
		loader.AddUnsignedInt("parentUID", parent->UID);
	else
		loader.AddUnsignedInt("parentUID", 0); 
	loader.AddString("Name", name.c_str());

	loader.AddUnsignedInt("isEnabled", isEnabled);
	loader.AddUnsignedInt("isStatic", isStatic);

	loader.AddUnsignedInt("HaveAABB", globalBoundingBox != nullptr && boundingBox != nullptr);

	if (globalBoundingBox != nullptr && boundingBox != nullptr)
	{
		//Save AABBs
		loader.AddVec3f("AABBMinPoint", boundingBox->minPoint);
		loader.AddVec3f("AABBMaxPoint", boundingBox->maxPoint);

		loader.AddVec3f("GlobalAABBMinPoint", globalBoundingBox->minPoint);
		loader.AddVec3f("GlobalAABBMaxPoint", globalBoundingBox->maxPoint);
	}


	//Save all components
	myTransform->OnSave(loader);

	loader.CreateComponentArray();
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		//Special save for Transform
		if ((*it)->myType == TRANSFORM)
			continue;
		
		loader.StartComponent();
		(*it)->OnSave(loader);
		loader.FinishComponent();
	}

	loader.FinishGameObject();
}

void GameObject::OnLoad(SceneLoader & loader)
{

	UID = loader.GetUnsignedInt("UID", 0);
	assert(UID != 0);

	name = loader.GetString("Name", "GameObject");

	isEnabled = loader.GetUnsignedInt("isEnabled", 0);
	isStatic = loader.GetUnsignedInt("isStatic", 0);

	//Special load for Transform
	CreateComponent(TRANSFORM);
	myTransform->OnLoad(loader);

	bool haveAABB = loader.GetUnsignedInt("HaveAABB", 0);
	if (haveAABB)
	{
		float3 minPoint = loader.GetVec3f("AABBMinPoint", float3(0, 0, 0));
		float3 maxPoint = loader.GetVec3f("AABBMaxPoint", float3(0, 0, 0));

		float3 globalMinPoint = loader.GetVec3f("GlobalAABBMinPoint", float3(0, 0, 0));
		float3 globalMaxPoint = loader.GetVec3f("GlobalAABBMaxPoint", float3(0, 0, 0));

		boundingBox = new AABB(minPoint, maxPoint);
		globalBoundingBox = new AABB(globalMinPoint, globalMaxPoint);
	}

	Component* component;
	while (loader.SelectNextComponent())
	{
		ComponentType type = (ComponentType)loader.GetUnsignedInt("Type", 0);
		assert(type != 0);
		
		component = CreateComponent(type);
		component->OnLoad(loader);
	}

	App->scene->UIDs.insert(UID);
	return;
}

float GameObject::IsIntersectedByRay(const float3 &origin, const LineSegment & ray)
{
	if (myMesh == nullptr)
		return -1.0f;

	//Transform ray coordinates into local space
	LineSegment localRay = LineSegment(ray);
	localRay.Transform(myTransform->globalModelMatrix.Inverted());
	

	return myMesh->IsIntersectedByRay(origin, localRay);
}

void GameObject::SetGlobalMatrix(const float4x4 & newGlobal)
{
	assert(myTransform != nullptr);
	if(parent != nullptr && parent->myTransform != nullptr)
	{
		myTransform->SetGlobalMatrix(newGlobal, parent->myTransform->globalModelMatrix);
	}

	return;
}

void GameObject::SetStatic()
{
	if(UID == 1)
	{
		isStatic = true;
		LOG("Root must be static. STOP!.");
		return;
	}

	//Get all GO related with this GO
	std::vector<GameObject*> myRelatedGO;
	GameObject* grandParent = this;
	while(grandParent->parent->UID != 1)
	{
		grandParent = grandParent->parent;
	}

	myRelatedGO.push_back(grandParent);

	//Get all children and set static boolean
	grandParent->GetAllChilds(myRelatedGO);
	for(auto go : myRelatedGO)
	{
		go->isStatic = isStatic;

		if (isStatic)
		{
			App->scene->dynamicGO.erase(go);
			App->scene->staticGO.insert(go);

			//Only added/removed to aabbtree if GO have mesh or is parent of mesh
			if((go->myMesh != nullptr || go->isParentOfMeshes) && go->globalBoundingBox != nullptr)
				App->scene->aabbTree->Remove(go);
		}

		else
		{
			App->scene->staticGO.erase(go);
			App->scene->dynamicGO.insert(go);

			if ((go->myMesh != nullptr || go->isParentOfMeshes) && go->globalBoundingBox != nullptr)
				App->scene->aabbTree->Insert(go);
		}

	}

	App->scene->BuildQuadTree();

	return;
}

void GameObject::GetAllChilds(std::vector<GameObject*>& allChilds)
{
	for(auto ch : children)
	{
		allChilds.push_back(ch);
		ch->GetAllChilds(allChilds);
	}

	return;
}

void GameObject::CheckDragAndDrop(GameObject * go)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
		ImGui::SetDragDropPayload("DRAG", &go, sizeof(GameObject*));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG");
		if (payload != nullptr) {
			GameObject* newChild = *reinterpret_cast<GameObject**>(payload->Data);

			GameObject* myParent = go;
			//While parent is not root
			while(myParent->UID != 1)
			{
				if(myParent->UID == newChild->UID)
				{
					LOG("It is not allowed to assign one of your children as a parent.");
					return;
				}
				myParent = myParent->parent;
			}

			newChild->SetParent(go);
			if(newChild->parent->myTransform != nullptr)
				newChild->myTransform->SetLocalMatrix(newChild->parent->myTransform->globalModelMatrix);
		}
		ImGui::EndDragDropTarget();
	}
}
