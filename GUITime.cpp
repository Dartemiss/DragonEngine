#include "GUITime.h"
#include "Application.h"
#include "ModuleTimeManager.h"


GUITime::GUITime()
{
}


GUITime::~GUITime()
{
}

void GUITime::Draw(const char * title, bool * p_opened)
{
	if (isEnabled)
	{
		ImGui::Begin(title, p_opened);
		ImGui::Text("Real Time Since Start: %.3f", App->timemanager->GetRealGameTime()/1000.0f); ImGui::SameLine();
		ImGui::Text("Real Time dt: %.3f", App->timemanager->GetRealDeltaTime() / 1000.0f);

		ImGui::Text("Game Time Since Start: %.3f", App->timemanager->GetGameTime()/1000.0f); ImGui::SameLine();
		ImGui::Text("Game Time dt: %.3f", App->timemanager->GetDeltaTime() / 1000.0f);
	}
}
