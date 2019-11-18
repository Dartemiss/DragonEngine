#include "GameObject.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

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
}

void GameObject::Update()
{
	
	return;
}

void GameObject::SetParent(GameObject * newParent)
{
	if(newParent != nullptr)
	{
		LOG("Setting new GamesObject parent and children.")
		parent = newParent;
		parent->children.push_back(this);
	}

	LOG("ERROR: Cannot set parent because new Parent is nullptr.");
	return;

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
			myMeshes = (ComponentMesh*)component;
			break;
		case MATERIAL:
			component = new ComponentMaterial();
			myMaterial = (ComponentMaterial*)component;
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

void GameObject::LoadModel(const char * path)
{
	if(myMeshes == nullptr || myMaterial == nullptr)
	{
		LOG("ERROR: Cannot load a model without a Mesh and a Material components");
		return;
	}

	myMeshes->LoadMeshes(path);

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
		//Hola
		App->scene->SelectObjectInHierarchy(this);
	}

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

void GameObject::UpdateTransform()
{
	if(myTransform != nullptr)
	{
		if(parent != nullptr && parent->myTransform != nullptr)
		{
			myTransform->SetGlobalMatrix(parent->myTransform->globalModelMatrix);
		}
		myTransform->UpdateMatrices();
	}
}
