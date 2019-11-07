#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "include/Geometry/Frustum.h"
#include <math.h>
#include "include/Math/float4.h"
#include "SDL.h"
#include "glew.h"


ModuleCamera::ModuleCamera()
{
	
}


ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	frustum = new Frustum();
	frustum->type = FrustumType::PerspectiveFrustum;
	frustum->pos = float3::zero;
	frustum->front = -float3::unitZ;
	frustum->up = float3::unitY;
	frustum->nearPlaneDistance = 0.1f;
	frustum->farPlaneDistance = 100.0f;
	frustum->verticalFov = (float)M_PI / 4.0f;
	aspect = (float)width / height;
	frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);

	frustum->Translate(float3(1.0f, 1.0f, 1.0f));
	
	
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	float3 mov = float3::zero;
	float3 rot = float3::zero;

	if(App->input->GetKey(SDL_SCANCODE_Q))
	{	
		mov += float3::unitY * 0.2;
	}

	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		mov -= float3::unitY * 0.2;
	}
	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		mov += float3::unitX * 0.2;
	}
	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		mov -= float3::unitX * 0.2;
	}
	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		mov -= float3::unitZ * 0.2;
	}
	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		mov += float3::unitZ * 0.2;
	}

	frustum->Translate(mov);

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

	math::float3x3 rotationY = math::float3x3::RotateY(rotY);
	math::float3x3 rotationX = math::float3x3::RotateX(rotX);
	

	frustum->front = rotationY.Transform(frustum->front).Normalized();
	//frustum-> = rotationX.Transform(frustum->front).Normalized();
	App->renderer->view = frustum->ViewMatrix();


	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetFOV(float FOV)
{
	float r = frustum->AspectRatio();
	frustum->verticalFov = FOV;
	frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f) *r);
	App->renderer->proj = frustum->ProjectionMatrix();
}

void ModuleCamera::SetAspectRatio()
{
	aspect = ((float)width / height);
	frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);
	App->renderer->proj = frustum->ProjectionMatrix();
}
