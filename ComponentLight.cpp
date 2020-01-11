#include "ComponentLight.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "SceneLoader.h"
#include "GL/glew.h"
#include "debugdraw.h"

ComponentLight::ComponentLight(GameObject * go)
{
	myGameObject = go;
	myType = LIGHT;
}

ComponentLight::ComponentLight(GameObject * go, ComponentLight * comp)
{
	myGameObject = go;
	myType = LIGHT;
}

ComponentLight::~ComponentLight()
{
}

void ComponentLight::DrawInspector()
{

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//TODO: Make that isActive is used
		ImGui::Checkbox("Active", &isActive);
		ImGui::SameLine();
		if (ImGui::Button("Remove Component", ImVec2(130, 20)))
		{
			LOG("Removing Component Light from %s", myGameObject->name);
			myGameObject->components.erase(std::find(myGameObject->components.begin(), myGameObject->components.end(), this));
			CleanUp();
			delete this;

			return;
		}

		if (lightType == LDIRECTIONAL)
		{
			ImGui::DragFloat("Azimuth (rad)", &azimuth, 0.01f, 0.0f, 3.1415f * 2);
			ImGui::DragFloat("Polar (rad)", &polar, 0.01f, 0.0f, 3.1415f * 2);

			ImGui::DragFloat3("Direction", &direction[0], 0.0f);
		}


	}
	return;
}

void ComponentLight::SetDrawLightsForMeshes(const unsigned int program)
{
	CalculateDirection();

	glProgramUniform3fv(program, glGetUniformLocation(program, "dirLight.direction"), 1, &direction[0]);
	glProgramUniform3fv(program, glGetUniformLocation(program, "dirLight.color"), 1, &color[0]);
}

void ComponentLight::Draw()
{
	if (lightType == LDIRECTIONAL)
	{
		float3 arrowFrom = myGameObject->myTransform->position;
		float3 arrowTo = arrowFrom + direction;
		dd::arrow(arrowFrom, arrowTo, float3(1.0f, 1.0f, 1.0f), 0.1f);
		
		float radius = 0.5f;
		float3 offset;
		for (float angle = 0; angle <= 360; angle += 45)
		{
			offset = float3(radius* sin(angle), 0.0f, radius * cos(angle));
			dd::arrow(arrowFrom + offset, arrowTo + offset, float3(1.0f, 1.0f, 1.0f), 0.1f);
		}
	}
}

void ComponentLight::OnSave(SceneLoader & loader)
{
}

void ComponentLight::OnLoad(SceneLoader & loader)
{
}

void ComponentLight::CalculateDirection()
{
	direction = float3(-sin(polar)*cos(azimuth), -cos(polar), -sin(polar)*sin(azimuth));
}
