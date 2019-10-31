#include "GUIWindow.h"
#include "Globals.h"




GUIWindow::GUIWindow()
{
	SetEnable(false);
}


GUIWindow::~GUIWindow()
{
}

void GUIWindow::Draw(const char * title, bool * p_opened, SDL_Window* window)
{
	if(isEnabled)
	{
		assert(window != nullptr);
		ImGui::Begin(title, p_opened);
		if (ImGui::Checkbox("FullScreen", &fullscreen))
		{
			if (fullscreen) 
			{

				SDL_DisplayMode displayMode;
				SDL_GetDesktopDisplayMode(0, &displayMode);
				SDL_SetWindowSize(window, displayMode.w, displayMode.h);
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
				//Resize(displayMode.w, displayMode.h);
			}
			else
			{
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_WIDTH);
				//Resize(SCREEN_WIDTH, SCREEN_WIDTH);

			}
		}
		ImGui::End();
	}

}
