#include "Globals.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glew.h"
#include <assert.h>

#define MAX_KEYS 300


ModuleInput::ModuleInput()
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate()
{
	mouse_motion = { 0, 0 };
	memset(windowEvents, false, WE_COUNT * sizeof(bool));
	mouse_wheel = 0;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}


	SDL_PumpEvents();
	static SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;
		
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				App->window->Resize(event.window.data1, event.window.data2);
			
			switch (event.window.event)
			{
					//case SDL_WINDOWEVENT_LEAVE:
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
				case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				case SDL_WINDOWEVENT_MAXIMIZED:
				case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				default:
					break;

			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event.motion.xrel;
			mouse_motion.y = event.motion.yrel;
			mouse.x = (float)event.motion.x / SCREEN_WIDTH;
			mouse.y = (float)event.motion.y / SCREEN_HEIGHT;
			
			break;
			
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0) // scroll up
			{
				mouse_wheel = event.wheel.y;
				App->camera->Zoom(true);
			}
			else if (event.wheel.y < 0) // scroll down
			{
				mouse_wheel = event.wheel.y;
				App->camera->Zoom(false);
			}
			break;

		case (SDL_DROPFILE): 
		    // In case if dropped file
			dropped_filedir = event.drop.file;
			// Shows directory of dropped file
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_INFORMATION,
				"File dropped on window",
				dropped_filedir,
				App->window->window
			);
			DropModelFile(dropped_filedir);
			SDL_free(dropped_filedir);    // Free dropped_filedir memory
			break;
			
		default:
			break;

		}
	}

	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update()
{
	SDL_PumpEvents();
	SDL_Event event;
	SDL_PollEvent(&event);
	ImGui_ImplSDL2_ProcessEvent(&event);
	if (event.type == SDL_QUIT)
		return UPDATE_STOP;
	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}


bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}
const fPoint& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const int ModuleInput::GetMouseWheel() const
{
	return mouse_wheel;
}

void ModuleInput::DropModelFile(char * dropped_filedir)
{
	LOG("Checking if extension of dropped file is correct.");
	assert(dropped_filedir != NULL);
	std::string fileExt(dropped_filedir);
	std::string filedir(dropped_filedir);
	std::size_t dotFound = fileExt.find_last_of(".");
	fileExt.erase(0, dotFound + 1);
	if(fileExt == "fbx" || fileExt == "FBX")
	{
		LOG("File is .fbx: Loading model.");

		App->modelLoader->loadModel(dropped_filedir);
		return;
	}

	if(fileExt == "png" || fileExt == "PNG" || fileExt == "JPG" || fileExt == "jpg" || fileExt == "DDS" || fileExt == "dds")
	{
		//Use texture
		App->modelLoader->ChangeTexture(dropped_filedir);
		return;
	}

	LOG("File is not an .fbx or texture invalid extension. File or texture cannot be loaded.");

	return;

}

const fPoint& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}