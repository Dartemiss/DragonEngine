#ifndef __GUIConsole_H__
#define __GUIConsole_H__

#include "Globals.h"
#include "GUI.h"
#include "imgui/imgui.h"

class GUIConsole : public GUI
{

private:
	bool ScrollToBottom = true;

public:
	GUIConsole() = default;
	~GUIConsole() = default;

	void Clear();

	ImGuiTextBuffer bufferConsole;
	
	void AddLog(const char* log);

	void Draw(const char* title);

};
#endif __GUIConsole_H__
