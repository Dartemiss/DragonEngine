#include "imgui/imgui.h"
#include "Globals.h"
#include "GUI.h"

class GUIWindow : public GUI
{
public:
	GUIWindow();
	~GUIWindow();

	void Draw(const char* title, bool* p_opened = NULL, SDL_Window* window = nullptr);


private:
	bool fullscreen = FULLSCREEN;
};

