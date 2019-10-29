#include "imgui/imgui.h"
#include "Globals.h"

class GUIConsole
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

