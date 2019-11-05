#ifndef __GUIAbout_H__
#define __GUIAbout_H__

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

#endif _GUIAbout_H__
