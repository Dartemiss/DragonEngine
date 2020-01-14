#include "ComponentTransform.h"
#include "MathGeoLib/Math/MathFunc.h"
#include "GameObject.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "SceneLoader.h"

ComponentTransform::ComponentTransform(GameObject* gameObject)
{
	myGameObject = gameObject;
	myType = TRANSFORM;
	UpdateMatrices();
}

ComponentTransform::ComponentTransform(GameObject * gameObject, ComponentTransform * comp)
{
	myGameObject = gameObject;
	myType = TRANSFORM;
	position = comp->position;
	rotation = comp->rotation;
	scale = comp->scale;
	UpdateMatrices();
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::EulerToQuat()
{
	rotation = math::Quat::FromEulerXYZ(DegToRad(eulerRotation).x, DegToRad(eulerRotation).y, DegToRad(eulerRotation).z);
}

void ComponentTransform::QuatToEuler()
{
	eulerRotation = rotation.ToEulerXYZ();
	eulerRotation.x = math::RadToDeg(eulerRotation.x);
	eulerRotation.y = math::RadToDeg(eulerRotation.y);
	eulerRotation.z = math::RadToDeg(eulerRotation.z);
}

void ComponentTransform::UpdateMatrices()
{
	globalModelMatrix = globalModelMatrix * localModelMatrix.Inverted();
	localModelMatrix = float4x4::FromTRS(position, rotation, scale);
	globalModelMatrix = globalModelMatrix * localModelMatrix;
}

void ComponentTransform::SetGlobalMatrix(const float4x4 &parentGlobal)
{
	globalModelMatrix = parentGlobal * localModelMatrix;
}

void ComponentTransform::SetGlobalMatrix(const float4x4 & newGlobal, const float4x4 & parentGlobal)
{
	globalModelMatrix = newGlobal;
	SetLocalMatrix(parentGlobal);
}

void ComponentTransform::SetLocalMatrix(const float4x4 &newParentGlobalMatrix)
{
	localModelMatrix = newParentGlobalMatrix.Inverted() *  globalModelMatrix;

	localModelMatrix.Decompose(position, rotation, scale);

	if (scale.x < 0.01f)
		scale.x = 0.01f;

	if (scale.y < 0.01f)
		scale.y = 0.01f;
	
	if (scale.z < 0.01f)
		scale.z = 0.01f;

	QuatToEuler();
	
}

void ComponentTransform::TranslateTo(const float3 & newPos)
{
	position = newPos;
}

void ComponentTransform::DrawInspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//Aux variables if static
		float3 auxPos = position;
		float3 auxRot = eulerRotation;
		float3 auxScale = scale;

		bool isStatic = myGameObject->isStatic;

		ImGui::Text("Position");
		ImGui::DragFloat3("Position", (float *)((isStatic) ? &auxPos : &position), 0.1f);
		ImGui::Text("Rotation");
		ImGui::DragFloat3("Rotation", (float *)((isStatic) ? &auxRot : &eulerRotation), 1.0f, -360.0f, 360.0f);
		ImGui::Text("Scale");
		ImGui::DragFloat3("Scale", (float *)((isStatic) ? &auxScale : &scale), 0.01f, 0.01f, 1000.0f);

		ImGui::Separator();

		ImGui::Text("AABB");
		if (myGameObject->globalBoundingBox == nullptr)
		{
			ImGui::Text("Is nullptr.");
		}
		else
		{
			ImGui::DragFloat3("Min Point", (float *)&myGameObject->globalBoundingBox->minPoint, 0.01f, 0.01f, 1000.0f);
			ImGui::DragFloat3("Max Point", (float *)&myGameObject->globalBoundingBox->maxPoint, 0.01f, 0.01f, 1000.0f);
		}

	}

	return;
}

void ComponentTransform::OnSave(SceneLoader & loader)
{
	loader.AddVec3f("Translation", position);
	loader.AddVec3f("Scale", scale);
	loader.AddVec4f("Rotation", float4(rotation.x, rotation.y, rotation.z, rotation.w));
}

void ComponentTransform::OnLoad(SceneLoader & loader)
{
	position = loader.GetVec3f("Translation", float3(0, 0, 0));
	scale = loader.GetVec3f("Scale", float3(1, 1, 1));
	float4 rotationVec = loader.GetVec4f("Rotation", float4(0, 0, 0, 1));
	rotation = Quat(rotationVec.x, rotationVec.y, rotationVec.z, rotationVec.w);

	QuatToEuler();
	UpdateMatrices();
}


