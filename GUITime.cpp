#include "GUITime.h"
#include "Application.h"
#include "ModuleTimeManager.h"

void GUITime::Draw(const char * title, bool * p_opened)
{
	if (isEnabled)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin(title, p_opened);
		ImGui::Text("Real Time Since Start: %.3f", App->timemanager->GetRealGameTime()/1000.0f); ImGui::SameLine();
		ImGui::Text("Real Time dt: %.3f", App->timemanager->GetRealDeltaTime() / 1000.0f);

		ImGui::Text("Game Time Since Start: %.3f", App->timemanager->GetGameTime()/1000.0f); ImGui::SameLine();
		ImGui::Text("Game Time dt: %.3f", App->timemanager->GetDeltaTime() / 1000.0f);

		ImGui::SliderFloat("Game Clock Scale", &App->timemanager->timeScale, 0.1f, 2.0f);
		ImGui::Text("FPS: %d", App->timemanager->FPS); ImGui::SameLine();
		ImGui::Text("Frame Count: %d", App->timemanager->frameCount);

		if(ImGui::Button("Play"))
		{
			App->timemanager->UnPauseGame();
		}

		if(ImGui::Button("Pause"))
		{
			App->timemanager->PauseGame();
		}

		if (ImGui::Button("Next Frames"))
		{
			App->timemanager->ExecuteNextFrames(2);
		}

		ImGui::End();

	}
}
