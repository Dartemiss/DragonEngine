#include "ModuleIMGUI.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleScene.h"
#include <stdio.h>
#include <SDL/SDL.h>


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
	const char* glsl_version = "#version 330";
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->glcontext);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	//Console is active by default
	//console.SetEnable(true);

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
	//ImGui::ShowDemoWindow();

	guiWindow.Draw("Window Configuration");
	console.Draw("Console");
	about.Draw("About");
	guiCamera.Draw("Camera Settings");
	timeManager.Draw("Timers");
	inspector.Draw("Properties");

	ImGui::BeginTabBar("MainViewTabs");
	ImGui::EndTabBar();

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
				about.ToggleEnable();
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
				console.ToggleEnable();
			}
			if (ImGui::MenuItem("Time"))
			{
				timeManager.ToggleEnable();
			}

			ImGui::EndMenu();
		}



		if(ImGui::BeginMenu("View"))
		{
			if(ImGui::MenuItem("Window"))
			{
				guiWindow.ToggleEnable();
			}

			if(ImGui::MenuItem("Camera"))
			{
				guiCamera.ToggleEnable();
			}

			if(ImGui::MenuItem("Properties"))
			{
				inspector.ToggleEnable();
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

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Save Scene"))
			{
				App->scene->SaveScene();
			}

			if (ImGui::MenuItem("Load Scene"))
			{
				App->scene->LoadScene();
			}

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

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
