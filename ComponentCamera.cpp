#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "GameObject.h"
#include <math.h>
#include "include/Geometry/Plane.h"
#include "glew.h"


ComponentCamera::ComponentCamera()
{
	myType = CAMERA;

	frustum = new Frustum();
	frustum->type = FrustumType::PerspectiveFrustum;
	frustum->pos = float3::zero;
	frustum->front = -float3::unitZ;
	frustum->up = float3::unitY;
	frustum->nearPlaneDistance = 0.1f;
	frustum->farPlaneDistance = 50.0f;
	frustum->verticalFov = (float)M_PI / 4.0f;
	aspect = 1.0f;
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);

	frustum->Translate(float3(0.0f, 0.0f, 0.0f));

	proj = frustum->ProjectionMatrix();
	view = frustum->ViewMatrix();

	DrawFrustum();
}


ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update()
{
	frustum->pos = myGameObject->myTransform->position;

	return;
}

void ComponentCamera::SetFOV()
{
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);
	proj = frustum->ProjectionMatrix();
}

void ComponentCamera::SetAspectRatio(int newHeight, int newWidth)
{
	aspect = ((float)newWidth / newHeight);
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f) *aspect);

	proj = frustum->ProjectionMatrix();
}

void ComponentCamera::Rotate(const float dx, const float dy)
{
	if (dx != 0.0f)
	{
		float3x3 rotationY = float3x3::RotateY(dx);
		frustum->front = rotationY.Transform(frustum->front).Normalized();
		frustum->up = rotationY.Transform(frustum->up).Normalized();
	}

	if (dy != 0.0f)
	{
		float3x3 rotationX = float3x3::RotateAxisAngle(frustum->WorldRight(), dy);
		frustum->up = rotationX.Transform(frustum->up).Normalized();
		frustum->front = rotationX.Transform(frustum->front).Normalized();
	}
}

void ComponentCamera::Move(float3 direction)
{
	if (!direction.Equals(float3::zero))
		frustum->Translate(direction);
}


void ComponentCamera::TranslateCameraToPoint(const float3 & newPos)
{
	frustum->pos = newPos;
	frustum->front = -float3::unitZ;
	frustum->up = float3::unitY;


	view = frustum->ViewMatrix();
}

void ComponentCamera::SetNearPlaneDistance(const float nearDist)
{
	frustum->nearPlaneDistance = nearDist;
}

void ComponentCamera::SetFarPlaneDistance(const float farDist)
{
	frustum->farPlaneDistance = farDist;
}

void ComponentCamera::LookAt(const float3 target)
{
	float3 dir = (target - frustum->pos).Normalized();
	float3x3 rot = float3x3::LookAt(frustum->front, dir, frustum->up, float3::unitY);
	frustum->front = rot.Transform(frustum->front).Normalized();
	frustum->up = rot.Transform(frustum->up).Normalized();
}

void ComponentCamera::DrawFrustum()
{

	//Draw Grid
	unsigned int progGrid = App->program->gridProg;
	glUseProgram(progGrid);

	float4x4 model = float4x4::identity.LookAt(-float3::unitZ, frustum->front, frustum->up, float3::unitY);

	glUniformMatrix4fv(glGetUniformLocation(progGrid,
		"model"), 1, GL_TRUE, &model[0][0]);

	//Temporary as std140 doesnt work
	glUniformMatrix4fv(glGetUniformLocation(progGrid,
		"proj"), 1, GL_TRUE, &App->camera->proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progGrid,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);



	//First we will get the width and height of the near plane
	float Hnear = 2 * tan(frustum->verticalFov / 2) * frustum->nearPlaneDistance;
	float Wnear = Hnear * aspect;

	//Then we do the same for the far plane
	float Hfar = 2 * tan(frustum->verticalFov / 2) * frustum->farPlaneDistance;
	float Wfar = Hfar * aspect;

	//Now we get the center of the planes
	float3 centerNear = frustum->pos + frustum->front * frustum->nearPlaneDistance;
	float3 centerFar = frustum->pos + frustum->front * frustum->farPlaneDistance;

	//And now we get our points
	float3 NearTopLeft = centerNear + (frustum->up * (Hnear / 2)) - (frustum->WorldRight() * (Wnear / 2));
	float3 NearTopRight = centerNear + (frustum->up * (Hnear / 2)) + (frustum->WorldRight() * (Wnear / 2));
	float3 NearBottomLeft = centerNear - (frustum->up * (Hnear / 2)) - (frustum->WorldRight() * (Wnear / 2));
	float3 NearBottomRight = centerNear - (frustum->up * (Hnear / 2)) + (frustum->WorldRight() * (Wnear / 2));

	float3 FarTopLeft = centerFar + (frustum->up * (Hfar / 2)) - (frustum->WorldRight() * (Wfar / 2));
	float3 FarTopRight = centerFar + (frustum->up * (Hfar / 2)) + (frustum->WorldRight() * (Wfar / 2));
	float3 FarBottomLeft = centerFar - (frustum->up * (Hfar / 2)) - (frustum->WorldRight() * (Wfar / 2));
	float3 aux1 = (frustum->up * (Hfar / 2));
	float3 aux2 = (frustum->WorldRight() * (Wfar / 2));
	float3 FarBottomRight = centerFar - aux1 + aux2;

	//Draw Lines
	glLineWidth(2.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	//Near Plane
	glVertex3f(NearTopLeft.x, NearTopLeft.y, NearTopLeft.z);
	glVertex3f(NearTopRight.x, NearTopRight.y, NearTopRight.z);

	glVertex3f(NearTopRight.x, NearTopRight.y, NearTopRight.z);
	glVertex3f(NearBottomRight.x, NearBottomRight.y, NearBottomRight.z);

	glVertex3f(NearBottomRight.x, NearBottomRight.y, NearBottomRight.z);
	glVertex3f(NearBottomLeft.x, NearBottomLeft.y, NearBottomLeft.z);

	glVertex3f(NearBottomLeft.x, NearBottomLeft.y, NearBottomLeft.z);
	glVertex3f(NearTopLeft.x, NearTopLeft.y, NearTopLeft.z);

	//Far Plane
	glVertex3f(FarTopLeft.x, FarTopLeft.y, FarTopLeft.z);
	glVertex3f(FarTopRight.x, FarTopRight.y, FarTopRight.z);

	glVertex3f(FarTopRight.x, FarTopRight.y, FarTopRight.z);
	glVertex3f(FarBottomRight.x, FarBottomRight.y, FarBottomRight.z);

	glVertex3f(FarBottomRight.x, FarBottomRight.y, FarBottomRight.z);
	glVertex3f(FarBottomLeft.x, FarBottomLeft.y, FarBottomLeft.z);

	glVertex3f(FarBottomLeft.x, FarBottomLeft.y, FarBottomLeft.z);
	glVertex3f(FarTopLeft.x, FarTopLeft.y, FarTopLeft.z);

	//Perpendicular planes
	glVertex3f(NearTopLeft.x, NearTopLeft.y, NearTopLeft.z);
	glVertex3f(FarTopLeft.x, FarTopLeft.y, FarTopLeft.z);

	glVertex3f(NearTopRight.x, NearTopRight.y, NearTopRight.z);
	glVertex3f(FarTopRight.x, FarTopRight.y, FarTopRight.z);

	glVertex3f(NearBottomLeft.x, NearBottomLeft.y, NearBottomLeft.z);
	glVertex3f(FarBottomLeft.x, FarBottomLeft.y, FarBottomLeft.z);

	glVertex3f(NearBottomRight.x, NearBottomRight.y, NearBottomRight.z);
	glVertex3f(FarBottomRight.x, FarBottomRight.y, FarBottomRight.z);


	glEnd();

	glUseProgram(0);

	return;
}
