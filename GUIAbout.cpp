#include "GUIAbout.h"

#define ENGINETITLE "Z Engine"
#define AUTHOR "Enrique Alexandre Gonzalez Sequeira"

GUIAbout::GUIAbout()
{
	SetEnable(false);
}


GUIAbout::~GUIAbout()
{
}

void GUIAbout::Draw(const char * title, bool * p_opened)
{
	if (isEnabled)
	{
		ImGui::Begin(title, p_opened);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Name: "); ImGui::SameLine();
		ImGui::Text(ENGINETITLE);
		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Description: "); ImGui::SameLine();
		ImGui::Text("Engine developed for UPC School masters degree videogame project.:"); 
		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Author: "); ImGui::SameLine();
		ImGui::Text(AUTHOR); 
		
		ImGui::Separator();

		ImGui::Text("Libraries: SDL, OpenGL, glew, MathGeoLib and IMGUI.");
		ImGui::Text("Licence: MIT");
		ImGui::End();
	}
}
