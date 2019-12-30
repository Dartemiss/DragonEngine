#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "ComponentCamera.h"
#include "Imgui/imgui.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float4.h"
#include "SDL/SDL.h"
#include "GL/glew.h"
#include <math.h>



ModuleCamera::ModuleCamera()
{
	
}


ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{

	editorCamera = new ComponentCamera(App->scene->GetRoot());
	editorCamera->SetFarPlaneDistance(300.0f);

	//UpdateUniformShaderMatrices();

	
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	UpdateUniformShaderMatrices();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	if (!SceneNotActive)
		return UPDATE_CONTINUE;

	float3 mov = float3::zero;

	if(App->input->GetKey(SDL_SCANCODE_Q))
	{	
		mov += editorCamera->frustum->up * movementSpeed;
	}

	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		mov -= editorCamera->frustum->up * movementSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		mov += editorCamera->frustum->WorldRight() * movementSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		mov -= editorCamera->frustum->WorldRight() * movementSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		mov += editorCamera->frustum->front * movementSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		mov -= editorCamera->frustum->front * movementSpeed;
	}

	

	float rotY = 0.0f;
	float rotX = 0.0f;
	//Rotation
	if (App->input->GetKey(SDL_SCANCODE_LEFT))
	{
		rotY = 0.02f;

	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
	{
		rotY = -0.02f;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP))
	{
		rotX = 0.02f;

	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN))
	{
		rotX = -0.02f;
	}

	//FPS ROTATION
	if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float dx = App->input->GetMouseMotion().x;
		float dy = App->input->GetMouseMotion().y;


		Rotate(-dx * rotationSpeed, -dy * rotationSpeed);
	}
	else
	{
		Rotate(rotY, rotX);
	}
	
	
	Move(mov);

	//If F is pressed, camera moves to the front of the model
	if(App->input->GetKey(SDL_SCANCODE_F))
	{
		TranslateCameraToPoint(App->modelLoader->correctCameraPositionForModel);
	}

	//If L is pressed, camera looks at model selected
	if(App->input->GetKey(SDL_SCANCODE_L))
	{
		LookAt(App->modelLoader->modelCenter);
	}

	//Alt + Left Click Orbit arround center of the objet selected
	if(App->input->GetKey(SDL_SCANCODE_LALT) && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		float dx = App->input->GetMouseMotion().x;
		float dy = App->input->GetMouseMotion().y;

		Orbit(-dx * rotationSpeed, -dy * rotationSpeed);
	}


	//Generate viewing matrix for camera movement/rotation
	editorCamera->ComputeViewMatrix();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	editorCamera->CleanUp();
	delete editorCamera;
	return true;
}

void ModuleCamera::SetFOV()
{
	editorCamera->frustum->horizontalFov = 2.0f * atanf(tanf(editorCamera->frustum->verticalFov * 0.5f) *editorCamera->aspect);
	editorCamera->ComputeProjMatrix();
}

void ModuleCamera::SetAspectRatio()
{
	editorCamera->aspect = ((float)App->window->width / App->window->height);
	editorCamera->frustum->horizontalFov = 2.0f * atanf(tanf(editorCamera->frustum->verticalFov * 0.5f) *editorCamera->aspect);
	editorCamera->ComputeProjMatrix();
}

void ModuleCamera::SetAspectRatio(int newWidth, int newHeight)
{
	editorCamera->aspect = ((float)newWidth / newHeight);
	editorCamera->frustum->horizontalFov = 2.0f * atanf(tanf(editorCamera->frustum->verticalFov * 0.5f) *editorCamera->aspect);

	editorCamera->ComputeProjMatrix();

}

void ModuleCamera::Rotate(const float dx, const float dy)
{
	
	if(dx != 0.0f)
	{
		float3x3 rotationY = float3x3::RotateY(dx);
		editorCamera->frustum->front = rotationY.Transform(editorCamera->frustum->front).Normalized();
		editorCamera->frustum->up = rotationY.Transform(editorCamera->frustum->up).Normalized();
	}

	if(dy != 0.0f)
	{
		float3x3 rotationX = float3x3::RotateAxisAngle(editorCamera->frustum->WorldRight(),dy);
		editorCamera->frustum->up = rotationX.Transform(editorCamera->frustum->up).Normalized();
		editorCamera->frustum->front = rotationX.Transform(editorCamera->frustum->front).Normalized();
	}

	return;
}

void ModuleCamera::Move(float3 direction)
{
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT))
	{
		direction *= 3.0f;
	}

	if(!direction.Equals(float3::zero))
		editorCamera->frustum->Translate(direction);

	return;
}

void ModuleCamera::Orbit(const float dx, const float dy)
{
	float3 center = App->modelLoader->modelCenter;

	if(dx != 0.0f)
	{
		float3x3 rot = float3x3::RotateY(dx);
		editorCamera->frustum->pos = rot.Transform(editorCamera->frustum->pos - center) + center;
	}

	if (dy != 0.0f)
	{
		float3x3 rot = float3x3::RotateAxisAngle(editorCamera->frustum->WorldRight(), dy);
		editorCamera->frustum->pos = rot.Transform(editorCamera->frustum->pos - center) + center;
	}

	LookAt(center);

}

void ModuleCamera::Zoom(const bool direction)
{
	if(direction)
	{
		Move(editorCamera->frustum->front);
	}
	else
	{
		Move(editorCamera->frustum->front * -1.0f);
	}

	return;
}

void ModuleCamera::TranslateCameraToPoint(const float3 & newPos)
{
	editorCamera->frustum->pos = newPos;
	editorCamera->frustum->front = -float3::unitZ;
	editorCamera->frustum->up = float3::unitY;

	LookAt(App->modelLoader->modelCenter);

	editorCamera->ComputeViewMatrix();

}

void ModuleCamera::SetNearPlaneDistance(const float nearDist)
{
	editorCamera->frustum->nearPlaneDistance = nearDist;
	editorCamera->ComputeProjMatrix();
}

void ModuleCamera::SetFarPlaneDistance(const float farDist)
{
	editorCamera->frustum->farPlaneDistance = farDist;
	editorCamera->ComputeProjMatrix();
}

void ModuleCamera::LookAt(const float3 target)
{
	float3 dir = (target - editorCamera->frustum->pos).Normalized();
	float3x3 rot = float3x3::LookAt(editorCamera->frustum->front, dir, editorCamera->frustum->up, float3::unitY);
	editorCamera->frustum->front = rot.Transform(editorCamera->frustum->front).Normalized();
	editorCamera->frustum->up = rot.Transform(editorCamera->frustum->up).Normalized();
}

void ModuleCamera::UpdateUniformShaderMatrices()
{
	glBindBuffer(GL_UNIFORM_BUFFER, App->program->uniformsBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float4x4), &editorCamera->proj[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, App->program->uniformsBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float4x4), sizeof(float4x4), &editorCamera->view[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

float4x4 ModuleCamera::GetProjMatrix() const
{
	return editorCamera->proj;
}

float4x4 ModuleCamera::GetViewMatrix() const
{
	return editorCamera->view;
}
