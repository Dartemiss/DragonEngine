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
	TexInfo texinfo = App->texture->textures[App->texture->indexTex];

	console.Draw("Console", &scrollDownConsole);
	about.Draw("About", &openAbout);
	guiWindow.Draw("Window Configuration", &showWindowConfig, App->window->window, texinfo);

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

		if(ImGui::BeginMenu("View"))
		{
			if(ImGui::MenuItem("Window"))
			{
				guiWindow.ToggleEnable();
			}

			if(ImGui::BeginMenu("Textures"))
			{
				if (ImGui::MenuItem("Lenna"))
				{
					App->texture->indexTex = 0;
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->texture->textures[0].TexWidth, App->texture->textures[0].TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, App->texture->textures[0].TexData);
				}
				if (ImGui::MenuItem("LET'S GO G2"))
				{
					App->texture->indexTex = 1;
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->texture->textures[1].TexWidth, App->texture->textures[1].TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, App->texture->textures[1].TexData);
				}
				if (ImGui::MenuItem("Dragon"))
				{
					App->texture->indexTex = 2;
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->texture->textures[2].TexWidth, App->texture->textures[2].TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, App->texture->textures[2].TexData);
				}
				ImGui::EndMenu();
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
