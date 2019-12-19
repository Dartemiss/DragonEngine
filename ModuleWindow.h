#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	bool fullscreen = FULLSCREEN;
	bool resizable = RESIZABLE;
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
	void Resize(unsigned int newWidth, unsigned int newHeight);
	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	//Update
	update_status Update();

	//FullScreen
	void setFullscreen(bool fullscrean);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	//Context
	SDL_GLContext glcontext = NULL;
	
};

#endif // __ModuleWindow_H__