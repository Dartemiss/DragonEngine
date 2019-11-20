#ifndef __GUIInspector_H__
#define __GUIInspector_H__

#include "Globals.h"
#include "GUI.h"
#include "imgui/imgui.h"

class GUIInspector : public GUI
{
public:
	GUIInspector() = default;
	~GUIInspector() = default;

	void Draw(const char* title);

};
#endif __GUIInspector_H__
