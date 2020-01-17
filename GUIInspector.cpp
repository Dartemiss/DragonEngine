#include "Application.h"
#include "GUIInspector.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "MathGeoLib/Math/float3.h"

void GUIInspector::Draw(const char * title)
{
	if(isEnabled)
	{
		ImGui::Begin(title, &isEnabled);
		ImGui::Text("Geometry");
		int aux = App->modelLoader->GetNumberOfMeshes();
		ImGui::DragInt("Number of meshes", (int *)&aux, 1, -1000, 1000);

		ImGui::Separator();

		ImGui::Text("Texture");
		//TODO: change mockup
		int numTex = 0;//App->modelLoader->numberOfTextures;
		ImGui::DragInt("Number of textures", (int *)&numTex, 1, -1000, 1000);



		ImGui::End();
	}
}
