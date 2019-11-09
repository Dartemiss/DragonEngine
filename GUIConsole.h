#ifndef __GUIConsole_H__
#define __GUIConsole_H__

#include "imgui/imgui.h"
#include "Globals.h"
#include "GUI.h"

class GUIConsole : public GUI
{

private:
	bool ScrollToBottom = true;

public:
	GUIConsole();
	~GUIConsole();

	void Clear();

	ImGuiTextBuffer bufferConsole;
	
	void AddLog(const char* log);

	void Draw(const char* title, bool* p_opened = NULL);

};
#endif __GUIConsole_H__