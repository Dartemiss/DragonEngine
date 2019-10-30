#include "GUIAbout.h"

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
		ImGui::Text("Name of the engine: Z Engine");
		ImGui::Text("Engine developed for UPC School masters degree videogame project.");
		ImGui::Text("Author: %s", AUTHOR);
		ImGui::Text("Libraries: SDL, OpenGL, glew, MathGeoLib and IMGUI.");
		ImGui::Text("Licence: MIT");
		ImGui::End();
	}
}
