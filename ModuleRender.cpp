#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "glew.h"
#include "include/Geometry/Frustum.h"
#include <math.h>
#include "include/Math/float4.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	GLenum err = glewInit();
	// … check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, 1024, 768);



	

	return true;
}

update_status ModuleRender::PreUpdate()
{
	int h;
	int w;
	SDL_GetWindowSize(App->window->window,
		&w,
		&h);

	glViewport(0,0,w,h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	DrawTriangle();
	//DrawRectangle();
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window

	return true;
}

void ModuleRender::DrawTriangle()
{
	float buffer_data[] = { -1.0f, -1.0f, 0.0f,
							 1.0f, -1.0f, 0.0f,
							 0.0f, 1.0f, 0.0f};
	int h;
	int w;
	SDL_GetWindowSize(App->window->window,
		&w,
		&h);

	//Frustum 
	//Projection Matrix
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = (float)M_PI / 4.0f;
	aspect = (float)w / h;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);
	float4x4 proj = frustum.ProjectionMatrix();

	model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f),float3x3::RotateY((float)M_PI / 4.0f), float3(1.0f,1.0f, 1.0f));
	float4x4 transform = proj * float4x4(model);

	//First parameter is eye position, second is target position
	float4x4 view = float4x4::LookAt(float3(0.0f,0.0f,-1.0f), math::float3(0.0f, 0.0f, 0.0f), math::float3(0.0f, 1.0f, 0.0f), math::float3(0.0f,1.0f, 0.0f));
	transform = proj *view*float4x4(model);

	float4 vertex0(buffer_data[0], buffer_data[1], buffer_data[2], 1.0f);
	float4 tmp = transform * vertex0;
	float3 final_vertex(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);
	buffer_data[0] = final_vertex.x;
	buffer_data[1] = final_vertex.y;
	buffer_data[2] = final_vertex.z;

	float4 vertex1(buffer_data[3], buffer_data[4], buffer_data[5], 1.0f);
	float4 tmp1 = transform * vertex1;
	float3 final_vertex1(tmp1.x / tmp1.w, tmp1.y / tmp1.w, tmp1.z / tmp1.w);
	buffer_data[3] = final_vertex1.x;
	buffer_data[4] = final_vertex1.y;
	buffer_data[5] = final_vertex1.z;

	float4 vertex2(buffer_data[6], buffer_data[7], buffer_data[8], 1.0f);
	float4 tmp2 = transform * vertex2;
	float3 final_vertex2(tmp2.x / tmp2.w, tmp2.y / tmp2.w, tmp2.z / tmp2.w);
	buffer_data[6] = final_vertex2.x;
	buffer_data[7] = final_vertex2.y;
	buffer_data[8] = final_vertex2.z;





	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0); // attribute 0
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0, // attribute 0
		3, // number of componentes (3 floats)
		GL_FLOAT, // data type
		GL_FALSE, // should be normalized?
		0, // stride
		(void*)0 // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 and 3 tris
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ModuleRender::DrawRectangle()
{
	float buffer_data_rect[] = { -1.5f, -0.5f, 0.0f,
								  1.5f, -0.5f, 0.0f,
								 -0.5f, 0.5f, 0.0f,
								  0.5f, 0.5f, 0.0f };
	unsigned int indices[] = {
		0,1,2,
		1,3,2
	};

	GLuint vbo;
	GLuint index;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data_rect), buffer_data_rect, GL_STATIC_DRAW);


	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	glVertexAttribPointer(
		0, // attribute 0
		3, // number of componentes (3 floats)
		GL_FLOAT, // data type
		GL_FALSE, // should be normalized?
		3* sizeof(float), // stride
		(void*)0 // array buffer offset
	);
	//glDrawArrays(GL_TRIANGLES, 0, 6); // start at 0 and 3 tris
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}

