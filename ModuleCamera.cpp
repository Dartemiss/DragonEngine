#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
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
	aspect = (float)App->window->width / App->window->height;
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
		mov += frustum->up * movementSpeed;
	}

	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		mov -= frustum->up * movementSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		mov += frustum->WorldRight() * movementSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		mov -= frustum->WorldRight() * movementSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		mov += frustum->front * movementSpeed;
	}
	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		mov -= frustum->front * movementSpeed;
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
	//Zoom
	wheelMovement = App->input->GetMouseWheel();
	if (wheelMovement > 1)
	{
		//mov += frustum->front * zoomSpeed;
	}
	else if(wheelMovement < 1)
	{
		//mov -= frustum->front * zoomSpeed;
	}


	Move(mov);




	//Generate viewing matrix for camera movement/rotation
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

void ModuleCamera::SetFOV()
{
	frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);
	App->renderer->proj = frustum->ProjectionMatrix();
}

void ModuleCamera::SetAspectRatio()
{
	aspect = ((float)App->window->width / App->window->height);
	frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);
	App->renderer->proj = frustum->ProjectionMatrix();
}

void ModuleCamera::Rotate(float dx, float dy)
{
	if(dx != 0.0f)
	{
		math::float3x3 rotationY = math::float3x3::RotateY(dx);
		frustum->front = rotationY.Transform(frustum->front).Normalized();
		frustum->up = rotationY.Transform(frustum->up).Normalized();
	}

	if(dy != 0.0f)
	{
		math::float3x3 rotationX = math::float3x3::RotateAxisAngle(frustum->WorldRight(),dy);
		frustum->up = rotationX.Transform(frustum->up).Normalized();
		frustum->front = rotationX.Transform(frustum->front).Normalized();
	}

	
}

void ModuleCamera::Move(float3 direction)
{
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT))
	{
		direction *= 3.0f;
	}

	frustum->Translate(direction);
}

void ModuleCamera::TranslateCameraToPoint(float3 & newPos)
{
	frustum->pos = newPos;
	App->renderer->view = frustum->ViewMatrix();
}
