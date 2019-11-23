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

	void Draw(const char* title, bool* p_opened);
	void ComputeRotation();

private:
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;
	
	bool computed = false;

};
#endif __GUIInspector_H__
