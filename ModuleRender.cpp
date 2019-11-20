#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleTimeManager.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "SDL.h"
#include "glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
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
		default: return "Unkown Source."; break;
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
		default: return "Unkown Type."; break;
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
		default: return "Unkown Severity"; break;
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

	model = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f), float3x3::RotateX(0.0f)* float3x3::RotateY(0.0f), float3(1.0f, 1.0f, 1.0f));


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

	//Draw Scene and Game Windows
	bool isEnabled = true;
	//First Scene window is created
	ImGui::Begin("Scene", &isEnabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 wSize = ImGui::GetWindowSize();
	App->camera->SetAspectRatio((int) wSize.x, (int) wSize.y);

	CreateFrameBuffer((int) wSize.x, (int) wSize.y);
	GenerateTexture((int)wSize.x, (int)wSize.y);


	widthScene = (int) wSize.x;
	heightScene = (int) wSize.y;

	ImGui::GetWindowDrawList()->AddImage(
		(void *)sceneTexture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(
			ImGui::GetCursorScreenPos().x + wSize.x,
			ImGui::GetCursorScreenPos().y + wSize.y
		),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);


	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	++App->timemanager->frameCount;
	App->timemanager->FinalDeltaTimes();
	App->timemanager->InitDeltaTimes();

	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteTextures(1, &sceneTexture);
	glDeleteRenderbuffers(1, &renderBufferObject);
	glDeleteFramebuffers(1, &frameBufferObject);
	//Destroy window

	return true;
}

void ModuleRender::DrawGrid()
{
	//Draw Grid
	unsigned int progGrid = App->program->gridProg;
	glUseProgram(progGrid);

	glUniformMatrix4fv(glGetUniformLocation(progGrid,
		"model"), 1, GL_TRUE, &model[0][0]);

	//Temporary as std140 doesnt work
	glUniformMatrix4fv(glGetUniformLocation(progGrid,
		"proj"), 1, GL_TRUE, &App->camera->proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progGrid,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);
	

	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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

	glUseProgram(0);
}

void ModuleRender::DrawAllGameObjects()
{
	unsigned int progModel = App->program->defaultProg;

	glUseProgram(progModel);

	//Temporary as std140 doesnt work
	glUniformMatrix4fv(glGetUniformLocation(progModel,
		"proj"), 1, GL_TRUE, &App->camera->proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progModel,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);

	for(auto gameObject : App->scene->allGameObjects)
	{
		glUniformMatrix4fv(glGetUniformLocation(progModel,
			"model"), 1, GL_TRUE, &gameObject->myTransform->globalModelMatrix[0][0]);

		for(auto mesh : gameObject->myMeshes->meshes)
		{
			mesh->Draw(progModel);
		}
	}


	glUseProgram(0);
}

void ModuleRender::DrawBoundingBoxes()
{
	//TODO: Separate the bounding box code into a different class
	if (App->modelLoader->isModelLoaded && showBoundingBox)
	{
		//Bounding Box
		glLineWidth(1.0f);
		float d = 200.0f;
		glBegin(GL_LINES);
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		//0->1
		for (int i = 0; i < 2; ++i)
		{
			//0->1 4->5
			glVertex3f(App->modelLoader->modelBox[0 + (i * 4)].x, App->modelLoader->modelBox[0 + (i * 4)].y, App->modelLoader->modelBox[0 + (i * 4)].z);
			glVertex3f(App->modelLoader->modelBox[1 + (i * 4)].x, App->modelLoader->modelBox[1 + (i * 4)].y, App->modelLoader->modelBox[1 + (i * 4)].z);

			//1->2 5->6
			glVertex3f(App->modelLoader->modelBox[1 + (i * 4)].x, App->modelLoader->modelBox[1 + (i * 4)].y, App->modelLoader->modelBox[1 + (i * 4)].z);
			glVertex3f(App->modelLoader->modelBox[2 + (i * 4)].x, App->modelLoader->modelBox[2 + (i * 4)].y, App->modelLoader->modelBox[2 + (i * 4)].z);

			//2->3 6->7
			glVertex3f(App->modelLoader->modelBox[2 + (i * 4)].x, App->modelLoader->modelBox[2 + (i * 4)].y, App->modelLoader->modelBox[2 + (i * 4)].z);
			glVertex3f(App->modelLoader->modelBox[3 + (i * 4)].x, App->modelLoader->modelBox[3 + (i * 4)].y, App->modelLoader->modelBox[3 + (i * 4)].z);

			//3->0 7->4
			glVertex3f(App->modelLoader->modelBox[0 + (i * 4)].x, App->modelLoader->modelBox[0 + (i * 4)].y, App->modelLoader->modelBox[0 + (i * 4)].z);
			glVertex3f(App->modelLoader->modelBox[3 + (i * 4)].x, App->modelLoader->modelBox[3 + (i * 4)].y, App->modelLoader->modelBox[3 + (i * 4)].z);
		}

		//Y lines
		//0->4
		glVertex3f(App->modelLoader->modelBox[0].x, App->modelLoader->modelBox[0].y, App->modelLoader->modelBox[0].z);
		glVertex3f(App->modelLoader->modelBox[4].x, App->modelLoader->modelBox[4].y, App->modelLoader->modelBox[4].z);

		//1->5
		glVertex3f(App->modelLoader->modelBox[1].x, App->modelLoader->modelBox[1].y, App->modelLoader->modelBox[1].z);
		glVertex3f(App->modelLoader->modelBox[5].x, App->modelLoader->modelBox[5].y, App->modelLoader->modelBox[5].z);

		//2->6
		glVertex3f(App->modelLoader->modelBox[2].x, App->modelLoader->modelBox[2].y, App->modelLoader->modelBox[2].z);
		glVertex3f(App->modelLoader->modelBox[6].x, App->modelLoader->modelBox[6].y, App->modelLoader->modelBox[6].z);

		//3->7
		glVertex3f(App->modelLoader->modelBox[3].x, App->modelLoader->modelBox[3].y, App->modelLoader->modelBox[3].z);
		glVertex3f(App->modelLoader->modelBox[7].x, App->modelLoader->modelBox[7].y, App->modelLoader->modelBox[7].z);

		glEnd();
	}
}

void ModuleRender::CreateFrameBuffer(int width, int height)
{
	if (width != widthScene || height != heightScene)
	{
		if (frameBufferObject == 0)
		{
			//Generate FrameBuffer if necessary
			glCreateFramebuffers(1, &frameBufferObject);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
		}

		if (sceneTexture != 0)
		{
			glDeleteTextures(1, &sceneTexture);
		}

		if(renderBufferObject != 0)
		{
			glDeleteRenderbuffers(1, &renderBufferObject);
		}

		glGenTextures(1, &sceneTexture);
		glBindTexture(GL_TEXTURE_2D, sceneTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

		//Generate RenderBuffers
		glGenRenderbuffers(1, &renderBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LOG("ERROR: Cannot create or render Scene framebuffer.");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void ModuleRender::GenerateTexture(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawGrid();
	DrawAllGameObjects();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



