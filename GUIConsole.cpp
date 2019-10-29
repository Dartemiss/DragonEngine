#include "GUIConsole.h"



GUIConsole::GUIConsole()
{
}


GUIConsole::~GUIConsole()
{
}

void GUIConsole::Clear()
{
	bufferConsole.clear(); 
}

void GUIConsole::AddLog(const char * log)
{
	bufferConsole.appendf(log);
}

void GUIConsole::Draw(const char * title, bool * p_opened)
{
	ImGui::Begin(title, p_opened);
	ImGui::TextUnformatted(bufferConsole.Buf.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();

}


