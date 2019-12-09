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
		ImGui::Text("Time for building recursive quadtree: %f", App->scene->timeRecursive);
		ImGui::Text("Time for building iterative quadtree: %f", App->scene->timeIterative);

		ImGui::Checkbox("Show Grid", &App->renderer->showGrid);
		ImGui::Checkbox("Show Bounding Box", &App->renderer->showBoundingBox);
		ImGui::Checkbox("Show QuadTree", &App->renderer->showQuadTree); 
		ImGui::Checkbox("Show AABBTree", &App->renderer->showAABBTree);
		ImGui::Checkbox("Show Frustum", &App->renderer->showFrustum);
		ImGui::Checkbox("Frusum Culling", &App->renderer->frustumCullingIsActivated);
		

		if(ImGui::Button("Build QuadTree"))
		{
			App->scene->BuildQuadTree();
		}

		if (ImGui::Button("Build AABBTree"))
		{
			App->scene->BuildAABBTree();
		}

		if(ImGui::Button("Generate Cubes"))
		{
			App->scene->CreateCubesScript();
		}

		ImGui::End();

	}


}
