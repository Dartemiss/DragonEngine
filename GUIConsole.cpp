#include "GUIConsole.h"


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
		ImGui::TextUnformatted(bufferConsole.Buf.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::End();
	}
}


