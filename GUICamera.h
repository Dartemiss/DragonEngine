#ifndef __GUICamera_H__
#define __GUICamera_H__
#include "imgui/imgui.h"
#include "Globals.h"
#include "GUI.h"


class Application;

class GUICamera : public GUI
{
public:
	GUICamera();
	~GUICamera();

	void Draw(const char* title, bool* p_opened);


	float FOV = 1.000f;

};
#endif __GUICamera_H__
