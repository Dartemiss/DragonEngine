#include "imgui/imgui.h"
#include "Globals.h"
#include "GUI.h"

class GUIAbout : public GUI
{
public:
	GUIAbout();
	~GUIAbout();

	void Draw(const char * title, bool * p_opened);
};

