#include "ModuleIMGUI.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
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
	//ImGui::ShowDemoWindow(&show_demo_window);
	if(App->texture->textures_loaded.size() > 0)
	{
		Texture *texinfo = &App->texture->textures_loaded[App->texture->indexTex];
		guiWindow.Draw("Window Configuration", &showWindowConfig, App->window->window, texinfo);
	}
	console.Draw("Console", &scrollDownConsole);
	about.Draw("About", &openAbout);
	guiCamera.Draw("Camera Settings", &showCameraGUI);
	timeManager.Draw("Timers", &showTimers);

	//Menu
	if (ImGui::BeginMainMenuBar()) 
	{

	
		if(ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
				ShellExecuteA(NULL, "open", DOCUMENTATION, NULL, NULL, SW_SHOWNORMAL);
			
			if(ImGui::MenuItem("About"))
			{
				openAbout = !openAbout;
			}
			ImGui::EndMenu();
		}

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

			if(ImGui::BeginMenu("Textures"))
			{
				if (ImGui::MenuItem("Lenna"))
				{
					App->texture->indexTex = 0;
					App->texture->executeTexImage2D();
				}
				if (ImGui::MenuItem("LET'S GO G2"))
				{
					App->texture->indexTex = 1;
					App->texture->executeTexImage2D();
				}
				if (ImGui::MenuItem("Dragon"))
				{
					App->texture->indexTex = 2;
					App->texture->executeTexImage2D();
				}
				ImGui::EndMenu();
			}

			if(ImGui::MenuItem("Camera"))
			{
				showCameraGUI = !showCameraGUI;
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
