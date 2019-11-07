#include "GUICamera.h"
#include "Application.h"
#include "ModuleCamera.h"


GUICamera::GUICamera()
{
	SetEnable(false);
}


GUICamera::~GUICamera()
{
}

void GUICamera::Draw(const char * title, bool * p_opened)
{
	if (isEnabled) 
	{
		ImGui::Begin(title, p_opened);

		ImGui::SliderFloat("FOV", &FOV, 0.100f, 3.000f, "%.3f");
		App->camera->SetFOV(FOV);


		ImGui::End();

	}


}
