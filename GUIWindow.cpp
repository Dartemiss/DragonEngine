#include "GUIWindow.h"
#include "Globals.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTimeManager.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ilu.h"


GUIWindow::GUIWindow()
{
	fpsTimer.StartTimer();
}

void GUIWindow::Draw(const char * title, bool * p_opened)
{
	if(isEnabled)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
		SDL_Window* window = App->window->window;
		assert(window != nullptr);
		ImGui::Begin(title, p_opened);
		if (ImGui::Checkbox("FullScreen", &App->window->fullscreen))
		{
			if (App->window->fullscreen)
			{
				SDL_DisplayMode displayMode;
				SDL_GetDesktopDisplayMode(0, &displayMode);
				SDL_SetWindowSize(window, displayMode.w, displayMode.h);
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
			ImGui::Text("Application average %.3f ms/frame (%d FPS)", 1000.0f / App->timemanager->FPS, App->timemanager->FPS);
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
				fps_log.push_back((float)App->timemanager->FPS);
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
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "CPUs: "); ImGui::SameLine();
			ImGui::Text("%d(Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "System RAM: "); ImGui::SameLine();
			ImGui::Text("%dGb", SDL_GetSystemRAM() / 1000);

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),"Caps: ");ImGui::SameLine();
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

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "GPU: "); ImGui::SameLine();
			ImGui::Text("%s", glGetString(GL_VENDOR));

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Brand: "); ImGui::SameLine();
			ImGui::Text("%s", glGetString(GL_RENDERER));

			#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
			#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

			GLint total_mem_kb = 0;
			glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
				&total_mem_kb);

			GLint cur_avail_mem_kb = 0;
			glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
				&cur_avail_mem_kb);

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),"VRAM Total: "); ImGui::SameLine();
			ImGui::Text("%d(Mb)", total_mem_kb / 1000);

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),"VRAM Available: "); ImGui::SameLine();
			ImGui::Text("%d(Mb)", cur_avail_mem_kb / 1000);

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),"VRAM Usage: "); ImGui::SameLine();
			ImGui::Text("%d(Mb)", (total_mem_kb / 1000) - (cur_avail_mem_kb / 1000));



		}
		if (ImGui::CollapsingHeader("All Textures Loaded"))
		{
			for (auto texInfo : App->texture->textures_loaded) 
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Path: "); ImGui::SameLine();
				ImGui::Text("%s", texInfo.path.c_str());

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Width: "); ImGui::SameLine();
				ImGui::Text("%d(Bytes)", texInfo.width);

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Height: "); ImGui::SameLine();
				ImGui::Text("%d(Bytes)", texInfo.width);


				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Depth: "); ImGui::SameLine();
				ImGui::Text("%d", texInfo.depth);

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Format: "); ImGui::SameLine();
				if (texInfo.format == IL_TYPE_UNKNOWN)
				{
					ImGui::Text("Unknown");
				}
				else if (texInfo.format == IL_PNG)
				{
					ImGui::Text("PNG");
				}
				else if (texInfo.format == IL_JPG)
				{
					ImGui::Text("JPG");
				}
				else if (texInfo.format == IL_DDS)
				{
					ImGui::Text("DDS");
				}

				ImGui::Separator();
			}


		}
		if (ImGui::CollapsingHeader("Variables"))
		{
			if (ImGui::CollapsingHeader("Renderer"))
			{
				//Face Culling
				ImGui::Checkbox("Face Culling", &App->renderer->faceCullingIsActive);
				App->renderer->EnableFaceCulling();

				//GL_DEPTH_TEST
				ImGui::Checkbox("Depth Test", &App->renderer->dephtTestIsActive);
				App->renderer->EnableDepthTest();

				//Changing front face
				ImGui::Checkbox("Front Face: CWW/CW", &App->renderer->changingFrontFace);
				App->renderer->ChangeFrontFace();
				//Texture2D
				ImGui::Checkbox("Texture2D", &App->renderer->texture2DIsActive);
				App->renderer->EnableTexture2D();

				//Fill triangles
				ImGui::Checkbox("Fill Triangles", &App->renderer->fillTrianglesIsActive);
				App->renderer->FillTriangles();

				//Alpha test
				ImGui::Checkbox("Alpha test", &App->renderer->alphaTestIsActive);
				App->renderer->EnableAlphaTest();

			}

			if (ImGui::CollapsingHeader("Input"))
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f,1.0f), "Mouse");
				ImGui::Text("Mouse Position: (%f , %f)", App->input->GetMousePosition().x, App->input->GetMousePosition().y);
				ImGui::Text("Mouse delta motion: (%f, %f)", App->input->GetMouseMotion().x, App->input->GetMouseMotion().y);
				ImGui::Text("Mouse Wheel: %d", App->input->GetMouseWheel());
				ImGui::Text("Mouse Right Click: %d", App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT ? 1 : 0);
				ImGui::Text("Mouse Left Click: %d", App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT ? 1 : 0);
				ImGui::Separator();

				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "KeyBoard");
				ImGui::Text("Keys down:");      for (int i = 0; i < 300; i++) if (App->input->GetKey(i) == KEY_DOWN) { ImGui::SameLine(); ImGui::Text("%d ", i); }
				ImGui::Text("Keys repeat:");      for (int i = 0; i < 300; i++) if (App->input->GetKey(i) == KEY_REPEAT) { ImGui::SameLine(); ImGui::Text("%d ", i); }
			}
		}

		ImGui::End();
	}



}
