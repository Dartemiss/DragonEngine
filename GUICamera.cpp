#include "GUICamera.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"



void GUICamera::Draw(const char * title)
{
	if (isEnabled) 
	{
		ImGui::Begin(title, &isEnabled);

		ImGui::SliderFloat("FOV", &App->camera->frustum->verticalFov, 0.100f, 3.000f, "%.3f");
		App->camera->SetFOV();

		ImGui::Text("Camera Position: (%.3f,%.3f,%.3f)", App->camera->frustum->pos.x, App->camera->frustum->pos.y, App->camera->frustum->pos.z);

		ImGui::Checkbox("Show Bounding Box", &App->renderer->showBoundingBox);

		ImGui::End();

	}


}
