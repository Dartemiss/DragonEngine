#include "GUITime.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleIMGUI.h"
#include "ModuleTimeManager.h"
#include "ModuleRender.h"
#include "FontAwesome/IconsFontAwesome5.h"
#include "SDL/SDL.h"

GUITime::GUITime()
{
	isEnabled = true;
}

void GUITime::Draw(const char * title)
{
	if (isEnabled)
	{
		ImGui::SetNextWindowPos(
			ImVec2(App->window->width * App->imgui->timerPosRatioWidth, App->window->height * App->imgui->timerPosRatioHeight)
		);
		ImGui::SetNextWindowSize(
			ImVec2(App->window->width * App->imgui->timerSizeRatioWidth, App->window->height * App->imgui->timerSizeRatioHeight)
		);

		ImGui::Begin(ICON_FA_CLOCK " Timers", &isEnabled);
		ImGui::Text("Real Time Since Start: %.3f", App->timemanager->GetRealGameTime()/1000.0f); ImGui::SameLine();
		ImGui::Text("Real Time dt: %.3f", App->timemanager->GetRealDeltaTime() / 1000.0f);

		ImGui::Text("Game Time Since Start: %.3f", App->timemanager->GetGameTime()/1000.0f); ImGui::SameLine();
		ImGui::Text("Game Time dt: %.3f", App->timemanager->GetDeltaTime() / 1000.0f);

		ImGui::SliderFloat("Game Clock Scale", &App->timemanager->timeScale, 0.1f, 2.0f);
		ImGui::Text("FPS: %d", App->timemanager->FPS); ImGui::SameLine();
		ImGui::Text("Frame Count: %d", App->timemanager->frameCount);

		ImGui::Checkbox("Fix FPS", &App->timemanager->fixFPS);
		ImGui::SliderInt("FPS", &App->timemanager->fixedFPS, 10, 60);


		if(ImGui::Checkbox("Vsync",&vsyncActive))
		{
			SDL_GL_SetSwapInterval(vsyncActive);
		}

		ImGui::Checkbox("MSAA", &App->renderer->antialiasing);

		ImGui::End();

	}
}
