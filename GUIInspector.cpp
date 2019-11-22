#include "Application.h"
#include "GUIInspector.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "include/Math/float3.h"

void GUIInspector::Draw(const char * title, bool * p_opened)
{
	if(isEnabled)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 700), ImGuiCond_FirstUseEver);
		ImGui::Begin(title, p_opened);
		ImGui::TextColored(ImVec4(1.0f,0.0,1.0f,1.0f), "Transform");
		ImGui::DragFloat3("Position (x,y,z)", (float *)&float3(0.0f, 0.0f, 0.0f), 0.1f, -1000.f, 1000.f);
		ImGui::DragFloat3("Rotation (x,y,z)", (float *)&float3(0.0f, 0.0f, 0.0f), 0.1f, -1000.f, 1000.f);
		ImGui::DragFloat3("Scale (x,y,z)", (float *)&App->renderer->model.GetScale(), 0.1f, -1000.f, 1000.f);

		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 0.0, 1.0f, 1.0f), "Geometry");

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Number of meshes: "); ImGui::SameLine();
		ImGui::Text("%d", App->modelLoader->GetNumberOfMeshes());

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Number of triangles: "); ImGui::SameLine();
		ImGui::Text("%d", App->modelLoader->GetNumberOfTriangles(true));

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Number of vertices: "); ImGui::SameLine();
		ImGui::Text("%d", App->modelLoader->GetNumberOfTriangles(false));
		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 0.0, 1.0f, 1.0f), "Current textures loaded");
		for(auto index : App->modelLoader->indicesOfCurrentTextures)
		{
			int window_width = (int) ImGui::GetWindowWidth();
			int window_height = (int) ImGui::GetWindowHeight();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture path: "); ImGui::SameLine();
			ImGui::Text("%s", App->texture->textures_loaded[index].path.c_str());
			ImGui::Image((ImTextureID)App->texture->textures_loaded[index].id, ImVec2(window_height * 0.5f, window_width * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		}
		
		ImGui::End();
	}
}
