#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
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
	
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = (float)M_PI / 4.0f;
	aspect = (float)width / height;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);
	
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	
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
	frustum.verticalFov = FOV;
	frustum.horizontalFov = 2.f * atanf(tanf(FOV * 0.5f) *aspect);
	App->renderer->proj = frustum.ProjectionMatrix();
}

void ModuleCamera::SetAspectRatio()
{
	aspect = ((float)width / height);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);
	App->renderer->proj = frustum.ProjectionMatrix();
}
