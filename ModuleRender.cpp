#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "SDL.h"
#include "glew.h"
#include "include/Geometry/Frustum.h"
#include <math.h>
#include "include/Math/float4.h"



const char* GetSourceStr(GLenum source)
{
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             return "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     return "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           return "Source: Other"; break;
	}

	return "Unkown Source";
}

const char* GetTypeStr(GLenum type)
{
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               return "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         return "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         return "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              return "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          return "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           return "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               return "Type: Other"; break;
	}

	return "Unkown Type";
}

const char* GetSeverityStr(GLenum severity)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         return "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       return "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          return "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "Severity: notification"; break;
	}

	return "Unkown Severity";
}


void APIENTRY OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	char tmp_string[4096];
	const char* tmp_source = GetSourceStr(source);
	const char* tmp_type = GetTypeStr(type);
	const char* tmp_severity = GetSeverityStr(severity);
	sprintf_s(tmp_string, 4095, "<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n",
		tmp_source, tmp_type, tmp_severity, id, message);
	OutputDebugString(tmp_string);
}



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
	glewInit();
	LOG("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//Debugging
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

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
	//Debugging
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glViewport(0, 0, 1024, 768);

	//Debug
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr); // sets the callback
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true); //filters notifications



	SDL_GetWindowSize(App->window->window,
		&width,
		&height);

	float buffer_data_rect[] = { -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
							  0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
							 -0.5f, 0.5f,  0.0f,  0.0f, 1.0f, // top left 
							  0.5f, 0.5f,  0.0f,  1.0f, 1.0f  // top right
	};


	unsigned int indices[] = {
		0,1,2,
		1,3,2
	};

	float buffer_data_triangle[] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
									  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
									  0.0f,  0.5f, 0.0f, 0.5f, 1.0f
	};


	//Create vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Create Buffer index
	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data_rect), buffer_data_rect, GL_STATIC_DRAW);


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, // attribute 0
		3, // number of componentes (3 floats)
		GL_FLOAT, // data type
		GL_FALSE, // should be normalized?
		5 * sizeof(float), // stride
		(void*)0 // array buffer offset
	);

	glEnableVertexAttribArray(0);

	//Project view model matrix and prog
	proj = App->camera->frustum->ProjectionMatrix();

	model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f), float3x3::RotateX((float)M_PI / 4.0f)* float3x3::RotateY((float)-M_PI / 4.0f), float3(1.0f, 1.0f, 1.0f));

	//First parameter is eye position, second is target position
	view = float4x4::LookAt(float3(0.0f, 0.0f, -1.0f), math::float3(0.0f, 0.0f, -1.0f), math::float3(0.0f, 1.0f, 0.0f), math::float3(0.0f, 1.0f, 0.0f));


	unsigned int vs = App->program->createVertexShader("../Shaders/VertexShader.vs");
	unsigned int fs = App->program->createFragmentShader("../Shaders/FragmentShader.fs");

	prog = App->program->createProgram(vs, fs);


	glUseProgram(prog);



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
	glUniformMatrix4fv(glGetUniformLocation(prog,
		"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(prog,
		"view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(prog,
		"proj"), 1, GL_TRUE, &proj[0][0]);


	//DrawTriangle();
	DrawRectangle();
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
	//float4x4 transform = proj * float4x4(model);

	//First parameter is eye position, second is target position
	float4x4 view = float4x4::LookAt(float3(0.0f,0.0f,-1.0f), math::float3(0.0f, 0.0f, -1.0f), math::float3(0.0f, 1.0f, 0.0f), math::float3(0.0f,1.0f, 0.0f));
	
	/*
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

	*/

	
	unsigned int vs = App->program->createVertexShader("../Shaders/VertexShader.vs");
	unsigned int fs = App->program->createFragmentShader("../Shaders/FragmentShader.fs");

	unsigned int prog = App->program->createProgram(vs, fs);

	glUseProgram(prog);
	glUniformMatrix4fv(glGetUniformLocation(prog,
		"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(prog,
		"view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(prog,
		"proj"), 1, GL_TRUE, &proj[0][0]);


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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	// red X
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
	glLineWidth(1.0f);


	
	//After use a vbo assign a 0 for efficency

}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}



