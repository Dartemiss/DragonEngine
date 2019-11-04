#include "imgui/imgui.h"
#include "Globals.h"
#include "GUI.h"
#include <vector>

class GUIWindow : public GUI
{
public:
	GUIWindow();
	~GUIWindow();

	void Draw(const char* title, bool* p_opened = NULL, SDL_Window* window = nullptr);
	std::vector<float> fps_log;
	std::vector<float> ms_log;

private:
	bool fullscreen = FULLSCREEN;
	bool resizable = RESIZABLE;
	float currentTime;
	float deltaTime;
	float previousTime;
	float timeCounter;

};

