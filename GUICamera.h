#ifndef __GUICamera_H__
#define __GUICamera_H__
#include "Globals.h"
#include "GUI.h"
#include "imgui/imgui.h"


class Application;

class GUICamera : public GUI
{
public:
	GUICamera() = default;
	~GUICamera() = default;

	void Draw(const char* title, bool* p_opened);


};
#endif __GUICamera_H__
