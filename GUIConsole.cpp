#include "GUIConsole.h"


GUIConsole::GUIConsole()
{
	width = 1120;
	height = 270;
}

void GUIConsole::Clear()
{
	bufferConsole.clear(); 
}

void GUIConsole::AddLog(const char * log)
{
	bufferConsole.appendf(log);
}

void GUIConsole::Draw(const char * title)
{
	if (isEnabled) 
	{
		ImGui::Begin(title, &isEnabled);

		if(firstTime)
		{
			firstTime = false;
			ImGui::SetWindowSize(ImVec2(width, height));
			ImGui::SetWindowPos(ImVec2(100,700));
			ImGui::End();
			return;
		}

		ImVec2 size = ImGui::GetWindowSize();
		if(size.x != width && size.y != height)
		{
			width = size.x;
			height = size.y;
			ImGui::SetWindowSize(ImVec2(width, height));
		}

		ImGui::TextUnformatted(bufferConsole.Buf.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::End();
	}
}


