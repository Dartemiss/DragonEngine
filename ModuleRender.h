#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "Globals.h"
#include "include/Math/float4x4.h"
#include "include/Math/float3x3.h"

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
	void DrawTriangle();
	void DrawRectangle();

	float aspect = 1.0f;

	float4x4 proj;
	float4x4 model;
	float4x4 view;

private:
	void* context;
};

#endif __ModuleRender_H__