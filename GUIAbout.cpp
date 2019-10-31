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
		ImGui::Text("Name:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ENGINETITLE);
		ImGui::Separator();

		ImGui::Text("Description:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f),"Engine developed for UPC School masters degree videogame project.");
		ImGui::Separator();

		ImGui::Text("Author:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), AUTHOR);
		ImGui::Separator();

		ImGui::Text("Libraries: SDL, OpenGL, glew, MathGeoLib and IMGUI.");
		ImGui::Text("Licence: MIT");
		ImGui::End();
	}
}
