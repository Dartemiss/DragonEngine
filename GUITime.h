#ifndef __GUITime_H__
#define __GUITime_H__

#include "Globals.h"
#include "GUI.h"


class Application;

class GUITime : public GUI
{
public:
	GUITime() = default;
	~GUITime() = default;

	void Draw(const char* title);

	

};

#endif __GUITime_H__
