#include "GUICamera.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleScene.h"



void GUICamera::Draw(const char * title)
{
	if (isEnabled) 
	{

		ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin(title, &isEnabled);

		ImGui::SliderFloat("FOV", &App->camera->frustum->verticalFov, 0.100f, 3.000f, "%.3f");
		App->camera->SetFOV();

		ImGui::Text("Camera Position: (%.3f,%.3f,%.3f)", App->camera->frustum->pos.x, App->camera->frustum->pos.y, App->camera->frustum->pos.z);
		ImGui::Text("Camera near distance: %.3f", App->camera->frustum->nearPlaneDistance);
		ImGui::Text("Camera far distance: %.3f", App->camera->frustum->farPlaneDistance);

		ImGui::Checkbox("Show Grid", &App->renderer->showGrid);
		ImGui::Checkbox("Show Bounding Box", &App->renderer->showBoundingBox);
		ImGui::Checkbox("Frusum Culling", &App->renderer->frustumCullingIsActivated);
		ImGui::Checkbox("Show QuadTree", &App->renderer->showQuadTree);

		if(ImGui::Button("Build QuadTree"))
		{
			App->scene->BuildQuadTree();
		}

		if(ImGui::Button("Generate Cubes"))
		{
			App->scene->CreateCubesScript();
		}

		ImGui::End();

	}


}
