#include "GUIWindow.h"
#include "Globals.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTimeManager.h"
#include "Application.h"
#include "ilu.h"


void GUIWindow::Draw(const char * title, bool * p_opened, SDL_Window* window, Texture *texInfo)
{
	if(isEnabled)
	{
		assert(window != nullptr);
		ImGui::Begin(title, p_opened);
		if (ImGui::Checkbox("FullScreen", &App->window->fullscreen))
		{
			if (App->window->fullscreen)
			{
				SDL_DisplayMode displayMode;
				SDL_GetDesktopDisplayMode(0, &displayMode);
				SDL_SetWindowSize(window, displayMode.w, displayMode.h);
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
				App->camera->SetAspectRatio();
				
			}
			else
			{
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_WIDTH);
				App->camera->SetAspectRatio();
				

			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("Resizable", &App->window->resizable);
		
		
		ImGui::SliderFloat("Brightness", &actualBright, 0.000f, 1.000f, "%.3f");
		ImGui::SliderInt("Width", &App->window->width, 1, 2500, "%d");
		ImGui::SliderInt("Height", &App->window->height, 1, 2500, "%d");

		if (App->window->resizable)
		{	
			SDL_SetWindowSize(window, App->window->width, App->window->height);
			App->camera->SetAspectRatio();
		}
		else
		{
			SDL_GetWindowSize(window, &App->window->width, &App->window->height);
			App->camera->SetAspectRatio();
		}
			

		SDL_SetWindowBrightness(window, actualBright);


		if(ImGui::CollapsingHeader("Framerate"))
		{
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / App->timemanager->FPS, App->timemanager->FPS);
			char title[25];
			if (fps_log.size() > 0)
			{
				sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);;
				ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			}

			if (ms_log.size() > 0)
			{
				sprintf_s(title, 25, "Miliseconds %0.1f", ms_log[ms_log.size() - 1]);;
				ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
			}

			if (fpsTimer.ReadTimer() > 1000)
			{
				fpsTimer.StopTimer();
				fps_log.push_back(App->timemanager->FPS);
				ms_log.push_back(1000.0f / App->timemanager->FPS);
				fpsTimer.StartTimer();
			}
			previousTime = currentTime;
			currentTime = (float)SDL_GetTicks();
			deltaTime = currentTime - previousTime;
			timeCounter += deltaTime;
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("CPUs: %d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
			ImGui::Text("System RAM: %dGb", SDL_GetSystemRAM()/1000);
			ImGui::Text("Caps: ");ImGui::SameLine();
			if(SDL_Has3DNow())
			{
				ImGui::Text("3DNow,"); ImGui::SameLine();
			}

			if(SDL_HasAVX())
			{
				ImGui::Text("AVX,"); ImGui::SameLine();
			}

			if (SDL_HasAVX2())
			{
				ImGui::Text("AVX2,"); ImGui::SameLine();
			}

			if(SDL_HasAltiVec())
			{
				ImGui::Text("AltiVec,"); ImGui::SameLine();
			}

			if(SDL_HasMMX())
			{
				ImGui::Text("MMX,"); ImGui::SameLine();
			}

			if(SDL_HasRDTSC())
			{
				ImGui::Text("RDTSC,"); ImGui::SameLine();
			}

			if(SDL_HasSSE())
			{
				ImGui::Text("SSE,"); ImGui::SameLine();
			}

			if (SDL_HasSSE2())
			{
				ImGui::Text("SSE2,"); ImGui::SameLine();
			}

			if (SDL_HasSSE3())
			{
				ImGui::Text("SSE3,"); ImGui::SameLine();
			}

			if (SDL_HasSSE41())
			{
				ImGui::Text("SSE41,"); ImGui::SameLine();
			}

			if (SDL_HasSSE42())
			{
				ImGui::Text("SSE42,"); ImGui::SameLine();
			}

			ImGui::Text("");

			ImGui::Separator();

			ImGui::Text("GPU: %s", glGetString(GL_VENDOR));
			ImGui::Text("Brand: %s", glGetString(GL_RENDERER));

			#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
			#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

			GLint total_mem_kb = 0;
			glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
				&total_mem_kb);

			GLint cur_avail_mem_kb = 0;
			glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
				&cur_avail_mem_kb);

			ImGui::Text("VRAM Total: %d(Mb)", total_mem_kb/1000);
			ImGui::Text("VRAM Available: %d(Mb)", cur_avail_mem_kb/1000);
			ImGui::Text("VRAM Usage: %d(Mb)", (total_mem_kb/1000) - (cur_avail_mem_kb / 1000));



		}
		if (ImGui::CollapsingHeader("Texture Data"))
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Width: "); ImGui::SameLine();
			ImGui::Text("%d(Bytes)", texInfo->width);

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Height: "); ImGui::SameLine();
			ImGui::Text("%d(Bytes)", texInfo->width);


			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Depth: "); ImGui::SameLine();
			ImGui::Text("%d", texInfo->depth);

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Format: "); ImGui::SameLine();
			if(texInfo->format == IL_TYPE_UNKNOWN)
			{
				ImGui::Text("Unknown");
			}
			else if(texInfo->format == IL_PNG)
			{
				ImGui::Text("PNG");
			}
			else if (texInfo->format == IL_JPG)
			{
				ImGui::Text("JPG");
			}
			else if (texInfo->format == IL_DDS)
			{
				ImGui::Text("DDS");
			}
			
		
		}

		ImGui::End();
	}



}
