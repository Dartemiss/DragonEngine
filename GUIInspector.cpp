#include "Application.h"
#include "GUIInspector.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"

void GUIInspector::Draw(const char * title)
{
	if(isEnabled)
	{
		ImGui::Begin(title, &isEnabled);
		ImGui::Text("Transform");
		ImGui::DragFloat3("Position (x,y,z)", (float *)&float3(0.0f, 0.0f, 0.0f), 0.1f, -1000.f, 1000.f);
		ImGui::DragFloat3("Rotation (x,y,z)", (float *)&float3(0.0f, 0.0f, 0.0f), 0.1f, -1000.f, 1000.f);
		ImGui::DragFloat3("Scale (x,y,z)", (float *)&App->renderer->model.GetScale(), 0.1f, -1000.f, 1000.f);

		ImGui::Separator();

		ImGui::Text("Geometry");
		int aux = App->modelLoader->GetNumberOfMeshes();
		ImGui::DragInt("Number of meshes", (int *)&aux, 1, -1000, 1000);

		ImGui::Separator();

		ImGui::Text("Texture");
		int numTex = App->modelLoader->numberOfTextures;
		ImGui::DragInt("Number of textures", (int *)&numTex, 1, -1000, 1000);



		ImGui::End();
	}
}
