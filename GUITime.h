#ifndef __GUITime_H__
#define __GUITime_H__

#include "Globals.h"
#include "GUI.h"


class Application;

class GUITime : public GUI
{
public:
	GUITime();
	~GUITime() = default;

	void Draw(const char* title);
	bool vsyncActive = true;

};

#endif __GUITime_H__
