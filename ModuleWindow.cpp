#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "GL/glew.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"

ModuleWindow::ModuleWindow()
{
	
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (RESIZABLE)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		width = (int)(DM.w);
		height = (int)(DM.h * 0.925);


		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		
		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}
	if(glcontext != NULL)
	{
		SDL_GL_DeleteContext(glcontext);
	}

	//Quit SDL subsystems
	//TODO: Fix error when closing SDL_Quit crashes : Destroying SDL window and quitting all SDL systemsExcepción producida en 0x8D000424 en DragonEngine.exe: 0xC0000005: Infracción de acceso al ejecutar la ubicación 0x8D000424.
	SDL_Quit();
	return true;
}

update_status ModuleWindow::Update()
{

	return UPDATE_CONTINUE;
}

void ModuleWindow::setFullscreen(bool fullscrean)
{

}

void ModuleWindow::Resize(unsigned int newWidth, unsigned int newHeight)
{
	width = newWidth;
	height = newHeight;
	App->camera->SetAspectRatio();
}

