#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "Globals.h"
#include "include/Math/float4x4.h"
#include "include/Math/float3x3.h"
#include "glew.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	void DrawRectangle();

	float aspect = 1.0f;

	float4x4 proj;
	float4x4 model;
	float4x4 view;

	unsigned int progLenna;
	unsigned int progGrid;
	unsigned int progModel;

	bool showBoundingBox = true;

	//Debug
	//void OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

private:
	void* context;

	//static float buffer_data_rect;
	//static unsigned int indices;
	int height = 0;
	int width = 0;

	unsigned int vbo;
	unsigned int index;

	



};

#endif __ModuleRender_H__