#include "ModuleIMGUI.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <stdio.h>
#include <SDL.h>


#define DOCUMENTATION "https://github.com/ocornut/imgui/wiki"


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
	bool show_demo_window = true;
	bool scrollDownConsole = true;
	bool openAbout = true;
	//ImGui::ShowDemoWindow(&show_demo_window);
	
	console.Draw("Console", &scrollDownConsole);
	about.Draw("About", &openAbout);

	//Menu
	if (ImGui::BeginMainMenuBar()) 
	{

	
		if(ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
				ShellExecuteA(NULL, "open", DOCUMENTATION, NULL, NULL, SW_SHOWNORMAL);
			
			if(ImGui::MenuItem("About"))
			{
				about.ToggleEnable();
			}
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Debug"))
		{
			if(ImGui::MenuItem("Console"))
			{
				console.ToggleEnable();
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
