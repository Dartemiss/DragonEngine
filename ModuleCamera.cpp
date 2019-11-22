#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"
#include "include/Geometry/Frustum.h"
#include "include/Math/float4.h"
#include "SDL.h"
#include "glew.h"
#include <math.h>


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
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);

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
		Move(mov);
	}
	else
	{
		Rotate(rotY, rotX);
	}
	
	

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
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);
	App->renderer->proj = frustum->ProjectionMatrix();
}

void ModuleCamera::SetAspectRatio()
{
	aspect = ((float)App->window->width / App->window->height);
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);
	App->renderer->proj = frustum->ProjectionMatrix();
}

void ModuleCamera::Rotate(const float dx, const float dy)
{
	
	if(dx != 0.0f)
	{
		float3x3 rotationY = float3x3::RotateY(dx);
		frustum->front = rotationY.Transform(frustum->front).Normalized();
		frustum->up = rotationY.Transform(frustum->up).Normalized();
	}

	if(dy != 0.0f)
	{
		float3x3 rotationX = float3x3::RotateAxisAngle(frustum->WorldRight(),dy);
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

	if(!direction.Equals(float3::zero))
		frustum->Translate(direction);
}

void ModuleCamera::Orbit(const float dx, const float dy)
{
	float3 center = App->modelLoader->modelCenter;

	if(dx != 0.0f)
	{
		float3x3 rot = float3x3::RotateY(dx);
		frustum->pos = rot.Transform(frustum->pos - center) + center;
	}

	if (dy != 0.0f)
	{
		float3x3 rot = float3x3::RotateAxisAngle(frustum->WorldRight(), dy);
		frustum->pos = rot.Transform(frustum->pos - center) + center;
	}

	LookAt(center);

}

void ModuleCamera::Zoom(const bool direction)
{
	if(direction)
	{
		Move(frustum->front);
	}
	else
	{
		Move(frustum->front * -1.0f);
	}

	return;
}

void ModuleCamera::TranslateCameraToPoint(const float3 & newPos)
{
	frustum->pos = newPos;
	frustum->front = -float3::unitZ;
	frustum->up = float3::unitY;

	LookAt(App->modelLoader->modelCenter);

	App->renderer->view = frustum->ViewMatrix();

}

void ModuleCamera::SetNearPlaneDistance(const float nearDist)
{
	frustum->nearPlaneDistance = nearDist;
}

void ModuleCamera::SetFarPlaneDistance(const float farDist)
{
	frustum->farPlaneDistance = farDist;
}

void ModuleCamera::LookAt(const float3 target)
{
	float3 dir = (target - frustum->pos).Normalized();
	float3x3 rot = float3x3::LookAt(frustum->front, dir, frustum->up, float3::unitY);
	frustum->front = rot.Transform(frustum->front).Normalized();
	frustum->up = rot.Transform(frustum->up).Normalized();
}
