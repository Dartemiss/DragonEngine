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
	ModuleRender() = default;
	~ModuleRender() = default;

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void DrawRectangle();

	float aspect = 1.0f;

	float4x4 model = float4x4::zero;

	bool showBoundingBox = false;

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

	//Draw
	void DrawGrid();
	void DrawAllGameObjects();
	



};

#endif __ModuleRender_H__