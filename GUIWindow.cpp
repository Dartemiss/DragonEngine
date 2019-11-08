#include "GUIWindow.h"
#include "Globals.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "Application.h"
#include "ilu.h"


GUIWindow::GUIWindow()
{
	SetEnable(false);
}


GUIWindow::~GUIWindow()
{
}

void GUIWindow::Draw(const char * title, bool * p_opened, SDL_Window* window, Texture *texInfo)
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
				
			}
			else
			{
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_WIDTH);
				

			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("Resizable", &resizable);
		
		
		ImGui::SliderFloat("Brightness", &actualBright, 0.000f, 1.000f, "%.3f");
		ImGui::SliderInt("Width", &width, 1, 2500, "%d");
		ImGui::SliderInt("Height", &heigth, 1, 2500, "%d");

		if (resizable) 
		{
			
			SDL_SetWindowSize(window, width, heigth);
			App->camera->width = width;
			App->camera->height = heigth;
			App->camera->SetAspectRatio();
		}
		else
		{
			SDL_GetWindowSize(window, &width, &heigth);
			App->camera->width = width;
			App->camera->height = heigth;
			App->camera->SetAspectRatio();
		}
			

		SDL_SetWindowBrightness(window, actualBright);


		if(ImGui::CollapsingHeader("Framerate"))
		{
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

			if (timeCounter > 1000)
			{
				timeCounter = 0.0f;
				fps_log.push_back(ImGui::GetIO().Framerate);
				ms_log.push_back(1000.0f / ImGui::GetIO().Framerate);
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
		if (ImGui::CollapsingHeader("Texture Options")) 
		{
			ImGui::Separator();
			ImGui::Text("Wrapping Options");
			ImGui::Text("");
			if (ImGui::Button("GL_REPEAT"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				App->texture->executeTexImage2D();
				
			}
			ImGui::SameLine();
			if (ImGui::Button("GL_MIRRORED_REPEAT"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				App->texture->executeTexImage2D();
			}

			if (ImGui::Button("GL_CLAMP_TO_EDGE"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				App->texture->executeTexImage2D();
			}
			ImGui::SameLine();
			if (ImGui::Button("GL_CLAMP_TO_BORDER"))
			{
				float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
				App->texture->executeTexImage2D();
			}
			ImGui::Separator();
			ImGui::Text("Filtering Options");
			ImGui::Text("");

			if (ImGui::Button("GL_NEAREST"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				App->texture->executeTexImage2D();
			}
			if (ImGui::Button("GL_LINEAR "))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				App->texture->executeTexImage2D();
				
			}

		}


		ImGui::End();
	}



}
