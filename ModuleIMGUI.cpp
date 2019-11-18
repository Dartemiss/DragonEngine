#include "ModuleIMGUI.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleScene.h"
#include <stdio.h>
#include <SDL.h>


#define DOCUMENTATION "https://github.com/ocornut/imgui/wiki"
#define REPOSITORY "https://github.com/Dartemiss/DragonEngine"


ModuleIMGUI::ModuleIMGUI()
{
}


ModuleIMGUI::~ModuleIMGUI()
{
}

bool ModuleIMGUI::Init()
{
	ImGui::CreateContext();
	const char* glsl_version = "#version 150";
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->glcontext);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleIMGUI::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleIMGUI::Update()
{
	if(App->texture->textures_loaded.size() > 0)
	{
		Texture *texinfo = &App->texture->textures_loaded[App->texture->indexTex];
		guiWindow.Draw("Window Configuration", &showWindowConfig, App->window->window, texinfo);
	}
	console.Draw("Console", &scrollDownConsole);
	about.Draw("About", &openAbout);
	guiCamera.Draw("Camera Settings", &showCameraGUI);
	timeManager.Draw("Timers", &showTimers);
	inspector.Draw("Properties", &showInspector);

	//Menu
	if (ImGui::BeginMainMenuBar()) 
	{

	
		if(ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Repository"))
				ShellExecuteA(NULL, "open", REPOSITORY, NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Documentation"))
				ShellExecuteA(NULL, "open", DOCUMENTATION, NULL, NULL, SW_SHOWNORMAL);
			
			if(ImGui::MenuItem("About"))
			{
				openAbout = !openAbout;
			}

			if(ImGui::MenuItem("Quit"))
			{
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
		}

		App->scene->DrawUIBarMenuGameObject();

		if(ImGui::BeginMenu("Debug"))
		{
			if(ImGui::MenuItem("Console"))
			{
				scrollDownConsole = !scrollDownConsole;
			}
			if (ImGui::MenuItem("Time"))
			{
				showTimers = !showTimers;
			}

			ImGui::EndMenu();
		}



		if(ImGui::BeginMenu("View"))
		{
			if(ImGui::MenuItem("Window"))
			{
				showWindowConfig = !showWindowConfig;
			}

			if(ImGui::MenuItem("Camera"))
			{
				showCameraGUI = !showCameraGUI;
			}

			if(ImGui::MenuItem("Properties"))
			{
				showInspector = !showInspector;
			}

			if(ImGui::MenuItem("Show Hierarchy"))
			{
				App->scene->showHierarchy = true;
			}

			if (ImGui::MenuItem("Show Inspector"))
			{
				App->scene->showInspector = true;
			}

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();


	guiCamera.SetEnable(showCameraGUI);
	guiWindow.SetEnable(showWindowConfig);
	timeManager.SetEnable(showTimers);
	console.SetEnable(scrollDownConsole);
	about.SetEnable(openAbout);
	inspector.SetEnable(showInspector);
	
	return UPDATE_CONTINUE;
}

update_status ModuleIMGUI::PostUpdate()
{

	//Render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleIMGUI::CleanUp()
{
	console.Clear();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	

	return true;
}
