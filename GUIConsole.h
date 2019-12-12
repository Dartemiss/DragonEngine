#ifndef __GUIConsole_H__
#define __GUIConsole_H__

#include "Globals.h"
#include "GUI.h"
#include "Dependencies/imgui/imgui.h"

class GUIConsole : public GUI
{

private:
	bool ScrollToBottom = true;

public:
	GUIConsole();
	~GUIConsole() = default;

	void Clear();

	ImGuiTextBuffer bufferConsole;
	
	void AddLog(const char* log);

	void Draw(const char* title);

	//Variables
	bool firstTime = true;

};
#endif __GUIConsole_H__
