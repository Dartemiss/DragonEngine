#ifndef __GUIAbout_H__
#define __GUIAbout_H__

#include "Globals.h"
#include "GUI.h"
#include "imgui/imgui.h"

class GUIAbout : public GUI
{
public:
	GUIAbout() = default;
	~GUIAbout() = default;

	void Draw(const char * title);
};

#endif _GUIAbout_H__
