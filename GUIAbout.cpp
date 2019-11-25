#include "GUIAbout.h"

#define ENGINETITLE "Dragon Engine"
#define AUTHOR "Enrique Alexandre Gonzalez Sequeira"


void GUIAbout::Draw(const char * title)
{
	if (isEnabled)
	{
		ImGui::Begin(title, &isEnabled);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Name: "); ImGui::SameLine();
		ImGui::Text(ENGINETITLE);
		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Description: "); ImGui::SameLine();
		ImGui::Text("Engine developed for UPC School masters degree videogame project.:"); 
		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Author: "); ImGui::SameLine();
		ImGui::Text(AUTHOR); 
		
		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Libraries: "); ImGui::SameLine();
		ImGui::Text("SDL2, OpenGL 4.3, glew 2.1.0 , MathGeoLib and IMGUI.");

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Licence: "); ImGui::SameLine();
		ImGui::Text("MIT.");
		ImGui::End();

	}
}
