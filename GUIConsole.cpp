#include "GUIConsole.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleIMGUI.h"
#include "FontAwesome/IconsFontAwesome5.h"

GUIConsole::GUIConsole()
{
	width = 1120;
	height = 270;
	isEnabled = true;
}

void GUIConsole::Clear()
{
	//bufferConsole.clear(); 
}

void GUIConsole::AddLog(const char * log)
{
	bufferConsole.appendf(log);
}

void GUIConsole::Draw()
{
	if (isEnabled) 
	{
		ImGui::SetNextWindowPos(
			ImVec2(1, App->window->height * App->imgui->consolePosRatioHeight)
			
		);
		ImGui::SetNextWindowSize(
			ImVec2(App->window->width * App->imgui->consoleSizeRatioWidth, App->window->height * App->imgui->consoleSizeRatioHeight)
		);
		ImGui::Begin(ICON_FA_TERMINAL " Console", &isEnabled);


		ImVec2 size = ImGui::GetWindowSize();
		
		if(size.x != width && size.y != height)
		{
			width = static_cast<int>(size.x);
			height = static_cast<int>(size.y);
			ImGui::SetWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)));
		}

		ImGui::TextUnformatted(bufferConsole.Buf.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;

		if (ImGui::Button("Clear"))
		{
			Clear();
		}

		ImGui::End();
	}
}


