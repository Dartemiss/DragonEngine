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

void ComponentLight::SetDirection(const float3 newDir)
{
	direction = newDir;
}

void ComponentLight::SetDrawLightsForMeshes(const unsigned int program)
{
	glProgramUniform3fv(program, glGetUniformLocation(program, "dirLight.direction"), 1, &direction[0]);
	glProgramUniform3fv(program, glGetUniformLocation(program, "dirLight.color"), 1, &color[0]);
}

void ComponentLight::Draw()
{
	if (lightType == LDIRECTIONAL)
	{
		float3 arrowFrom = myGameObject->myTransform->position;
		float3 arrowTo = arrowFrom + direction;
		dd::arrow(arrowFrom, arrowTo, float3(1.0f, 1.0f, 1.0f), 1.0f);
	}
}

void ComponentLight::OnSave(SceneLoader & loader)
{
}

void ComponentLight::OnLoad(SceneLoader & loader)
{
}
