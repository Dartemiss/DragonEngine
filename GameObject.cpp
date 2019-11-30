#include "GameObject.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL.h"

GameObject::GameObject()
{
}

GameObject::GameObject(const char * name)
{
	this->name = name;
	CreateComponent(TRANSFORM);

}


GameObject::~GameObject()
{
	delete boundingBox;
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

		return;
	}



	LOG("ERROR: Cannot set parent because new Parent is nullptr.");
	return;

}

void GameObject::RemoveChildren(GameObject * child)
{

	if(!children.empty())
	{
		children.erase(std::remove(children.begin(), children.end(), child), children.end());
	}
	
	return;
}

void GameObject::DeleteGameObject()
{
	parent->RemoveChildren(this);
	App->scene->RemoveGameObject(this);
	CleanUp();
}

void GameObject::CleanUp()
{
	for(auto comp : components)
	{
		delete comp;
	}
	
	delete boundingBox;
	delete globalBoundingBox;

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
			component = new ComponentMesh();
			myMesh = (ComponentMesh*)component;
			break;
		case MATERIAL:
			component = new ComponentMaterial();
			myMaterial = (ComponentMaterial*)component;
			break;

		case CAMERA:
			component = new ComponentCamera();
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
			//TODO: Copy gameobjects
		}
		if (ImGui::Selectable("Paste"))
		{
			//TODO: Paste gameobjects
		}
		
		ImGui::Separator();

		if (ImGui::Selectable("Rename"))
		{
			//TODO: Rename gameobjects
		}
		if (ImGui::Selectable("Duplicate"))
		{
			//TODO: Duplicate gameobjects
		}

		if (ImGui::Selectable("Delete"))
		{
			//TODO: Delete gameobjects
			//DeleteGameObject();
		}

		ImGui::Separator();

		if(ImGui::Selectable("Create Empty"))
		{
			//Create empty gameobject
			App->scene->CreateEmpy(this);
		}

		if (ImGui::BeginMenu("Create 3D Object"))
		{
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

			if (ImGui::MenuItem("Baker House"))
			{
				// TODO :CreateGameObjectBakerHouse();
				App->scene->CreateGameObjectBakerHouse(this);
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
			ComponentCamera* mainCamera = (ComponentCamera*)comp;
			mainCamera->DrawFrustum();
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

			float3 globalPos, globalScale;
			float3x3 globalRot;
			myTransform->globalModelMatrix.Decompose(globalPos, globalRot, globalScale);

			globalBoundingBox->minPoint = (boundingBox->minPoint + globalPos);
			globalBoundingBox->maxPoint = (boundingBox->maxPoint + globalPos);
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
	float3 min = float3::zero;
	float3 max = float3::zero;

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
		globalBoundingBox = new AABB(min + globalPos, max + globalPos);

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
	globalBoundingBox = new AABB(min + globalPos, max + globalPos);

	return;
}

void GameObject::DrawAABB() const
{
	//Bounding Box
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	//0->1 
	glVertex3f(boundingBox->CornerPoint(0).x, boundingBox->CornerPoint(0).y, boundingBox->CornerPoint(0).z);
	glVertex3f(boundingBox->CornerPoint(1).x, boundingBox->CornerPoint(1).y, boundingBox->CornerPoint(1).z);

	//1->5
	glVertex3f(boundingBox->CornerPoint(1).x, boundingBox->CornerPoint(1).y, boundingBox->CornerPoint(1).z);
	glVertex3f(boundingBox->CornerPoint(5).x, boundingBox->CornerPoint(5).y, boundingBox->CornerPoint(5).z);

	//5-4
	glVertex3f(boundingBox->CornerPoint(5).x, boundingBox->CornerPoint(5).y, boundingBox->CornerPoint(5).z);
	glVertex3f(boundingBox->CornerPoint(4).x, boundingBox->CornerPoint(4).y, boundingBox->CornerPoint(4).z);

	//4-0
	glVertex3f(boundingBox->CornerPoint(0).x, boundingBox->CornerPoint(0).y, boundingBox->CornerPoint(0).z);
	glVertex3f(boundingBox->CornerPoint(4).x, boundingBox->CornerPoint(4).y, boundingBox->CornerPoint(4).z);
	
	//----//

	//2->3 
	glVertex3f(boundingBox->CornerPoint(2).x, boundingBox->CornerPoint(2).y, boundingBox->CornerPoint(2).z);
	glVertex3f(boundingBox->CornerPoint(3).x, boundingBox->CornerPoint(3).y, boundingBox->CornerPoint(3).z);

	//3->7
	glVertex3f(boundingBox->CornerPoint(3).x, boundingBox->CornerPoint(3).y, boundingBox->CornerPoint(3).z);
	glVertex3f(boundingBox->CornerPoint(7).x, boundingBox->CornerPoint(7).y, boundingBox->CornerPoint(7).z);

	//7-6
	glVertex3f(boundingBox->CornerPoint(7).x, boundingBox->CornerPoint(7).y, boundingBox->CornerPoint(7).z);
	glVertex3f(boundingBox->CornerPoint(6).x, boundingBox->CornerPoint(6).y, boundingBox->CornerPoint(6).z);

	//6-2
	glVertex3f(boundingBox->CornerPoint(6).x, boundingBox->CornerPoint(6).y, boundingBox->CornerPoint(6).z);
	glVertex3f(boundingBox->CornerPoint(2).x, boundingBox->CornerPoint(2).y, boundingBox->CornerPoint(2).z);

	
	//Y lines
	//0->2
	glVertex3f(boundingBox->CornerPoint(0).x, boundingBox->CornerPoint(0).y, boundingBox->CornerPoint(0).z);
	glVertex3f(boundingBox->CornerPoint(2).x, boundingBox->CornerPoint(2).y, boundingBox->CornerPoint(2).z);

	//1->3
	glVertex3f(boundingBox->CornerPoint(1).x, boundingBox->CornerPoint(1).y, boundingBox->CornerPoint(1).z);
	glVertex3f(boundingBox->CornerPoint(3).x, boundingBox->CornerPoint(3).y, boundingBox->CornerPoint(3).z);

	//5->7
	glVertex3f(boundingBox->CornerPoint(5).x, boundingBox->CornerPoint(5).y, boundingBox->CornerPoint(5).z);
	glVertex3f(boundingBox->CornerPoint(7).x, boundingBox->CornerPoint(7).y, boundingBox->CornerPoint(7).z);

	//6->4
	glVertex3f(boundingBox->CornerPoint(6).x, boundingBox->CornerPoint(6).y, boundingBox->CornerPoint(6).z);
	glVertex3f(boundingBox->CornerPoint(4).x, boundingBox->CornerPoint(4).y, boundingBox->CornerPoint(4).z);
	

	glEnd();
}

void GameObject::DrawInspector(bool &showInspector)
{
	ImGui::Begin("Inspector", &showInspector);

	ImGui::Checkbox("", &isEnabled); ImGui::SameLine();
	
	char* go_name = new char[64];
	strcpy(go_name, name.c_str());
	if(ImGui::InputText("##Name", go_name, 64))
	{
		name = std::string(go_name);
	}
	ImGui::SameLine();

	delete[] go_name;

	ImGui::Checkbox("Static", &isStatic);


	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position");
		ImGui::DragFloat3("Position", (float *)&myTransform->position, 0.1f);
		ImGui::Text("Rotation");
		ImGui::DragFloat3("Rotation", (float *)&myTransform->eulerRotation, 1.0f, -360.0f, 360.0f);
		ImGui::Text("Scale");
		ImGui::DragFloat3("Scale", (float *)&myTransform->scale, 0.01f, 0.01f, 1000.0f);

	}
	ImGui::End();
	//Change EulerRotation to Quat
	myTransform->EulerToQuat();
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
			GameObject * newChild = *reinterpret_cast<GameObject**>(payload->Data);
			newChild->SetParent(go);
			if(newChild->parent->myTransform != nullptr)
				newChild->myTransform->SetLocalMatrix(newChild->parent->myTransform->globalModelMatrix);
		}
		ImGui::EndDragDropTarget();
	}
}
