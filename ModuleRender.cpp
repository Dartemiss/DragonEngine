#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleTimeManager.h"
#include "ModuleIMGUI.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "ModuleTexture.h"
#include "ModuleDebugDraw.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "MyQuadTree.h"
#include "AABBTree.h"
#include "debugdraw.h"
#include "Skybox.h"
#include "SDL/SDL.h"
#include "GL/glew.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include <math.h>
#include "MathGeoLib/Math/float4.h"
#include "Brofiler/Brofiler.h"
#include "ImGuizmo/ImGuizmo.h"
#include "FontAwesome/IconsFontAwesome5.h"
#include "FontAwesome/IconsFontAwesome5Brands.h"




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

	App->window->glcontext = SDL_GL_CreateContext(App->window->window);

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


	//Project view model matrix and prog
	for (auto comp : App->scene->mainCamera->components)
	{
		if (comp->myType == CAMERA)
		{
			gameCamera = (ComponentCamera*)comp;
		}
	}

	//Skybox

	skybox = new Skybox();

	//TODO: move to texture Start
	App->texture->LoadWhiteFallbackTexture();

	//Scene w, h
	widthScene = static_cast<int>(App->window->width * App->imgui->sceneSizeRatioWidth);
	heightScene = static_cast<int>(App->window->height * App->imgui->sceneSizeRatioHeight);

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
	//Use this line to compute information about this function
	BROFILER_CATEGORY("Update", Profiler::Color::Orchid);
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0, 1, 0, 1));
	
	ImGui::SetNextWindowPos(
		ImVec2(App->window->width * App->imgui->tabsPosRatioWidth,App->window->height * App->imgui->tabsPosRatioHeight)
		
	);
	ImGui::SetNextWindowSize(
		ImVec2(App->window->width * App->imgui->tabsSizeRatioWidth, App->window->height * App->imgui->tabsSizeRatioHeight)
		
	);

	//Draw Scene and Game Windows
	if (ImGui::Begin("MainView", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
	{
		ImGui::BeginTabBar("");

		if (ImGui::BeginTabItem(ICON_FA_BUILDING " Scene") && !isGamePlaying)
		{
			DrawSceneBuffer();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(ICON_FA_GAMEPAD  " Game") || isGamePlaying)
		{
			DrawGameBuffer();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();

		//Draw Scene or game depending

		//Draw Buttons
		App->imgui->DrawPlayPauseButtons();

	}
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();


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
	delete skybox;

	LOG("Destroying renderer");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteTextures(1, &sceneTexture);
	glDeleteRenderbuffers(1, &renderBufferObject);
	glDeleteFramebuffers(1, &frameBufferObject);
	//Destroy window

	return true;
}

void ModuleRender::DrawGuizmo() const
{
	ImVec2 pos = ImGui::GetWindowPos();
	ImGuizmo::SetRect((float)pos.x, (float)pos.y, (float)widthScene, (float)heightScene);
	ImGuizmo::SetDrawlist();

	DrawGuizmoButtons();

	ImGui::SetCursorPos({ 20,30 });

	//Chose which guizmo we will use
	if(App->scene->selectedByHierarchy != nullptr && !App->scene->selectedByHierarchy->isStatic)
	{
		//Use guizmos only if object is static
		ImGuizmo::Enable(true);
		//TODO: Fix a bug where if you modify first rotation and then you use guizmo to scale rotation comeback to default (related with ImGuizmo::MODE = WORLD)
		float4x4 model = App->scene->selectedByHierarchy->myTransform->globalModelMatrix;
		float4x4 view = App->camera->GetViewMatrix();
		float4x4 proj = App->camera->GetProjMatrix();

		ImGuizmo::SetOrthographic(false);

		model.Transpose();
		view.Transpose();
		proj.Transpose();

		ImGuizmo::Manipulate((float *)&view, (float *)&proj, (ImGuizmo::OPERATION)currentOperation, (ImGuizmo::MODE)currentMode, (float*)&model, NULL, NULL,NULL,NULL);

		//Assign new model matrix
		if(ImGuizmo::IsUsing())
		{
			model.Transpose();
			App->scene->selectedByHierarchy->SetGlobalMatrix(model);
		}

	}

	return;
}

void ModuleRender::DrawAllGameObjects()
{

	//unsigned int progModel = App->program->defaultProg;
	unsigned int progModel = App->program->uberProg;

	glUseProgram(progModel);

	//Temporary as std140 doesnt work
	glUniformMatrix4fv(glGetUniformLocation(progModel,
		"proj"), 1, GL_TRUE, &App->camera->editorCamera->proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progModel,
		"view"), 1, GL_TRUE, &App->camera->editorCamera->view[0][0]);

	std::set<GameObject*> staticGO;
	std::set<GameObject*> dynamicGO;
	
	if(App->scene->quadtreeIsComputed)
	{
		App->scene->quadtree->GetIntersection(staticGO, &App->camera->editorCamera->frustum->MinimalEnclosingAABB());
	}
	App->scene->aabbTree->GetIntersection(dynamicGO, &App->camera->editorCamera->frustum->MinimalEnclosingAABB());

	//With C++ 17 we could do staticGO.merge(dynamicGO);
	std::set<GameObject*> onCameraGO = staticGO;
	onCameraGO.insert(dynamicGO.begin(), dynamicGO.end());

	for(auto gameObject : onCameraGO)
	{
		if (!gameObject->isEnabled)
			continue;

		glUniformMatrix4fv(glGetUniformLocation(progModel,
			"model"), 1, GL_TRUE, &gameObject->myTransform->globalModelMatrix[0][0]);

		if(gameObject->globalBoundingBox != nullptr)
		{

			if(App->camera->editorCamera->AABBWithinFrustum(*gameObject->globalBoundingBox) != 0)
			{

				gameObject->Draw(progModel, false, showBoundingBox);
			}

		}
		else
		{
			gameObject->Draw(progModel, false, showBoundingBox);
		}



	}


	glUseProgram(0);
}

void ModuleRender::DrawGame()
{
	unsigned int progModel = App->program->uberProg;
	glUseProgram(progModel);

	glUniformMatrix4fv(glGetUniformLocation(progModel,
		"proj"), 1, GL_TRUE, &gameCamera->proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progModel,
		"view"), 1, GL_TRUE, &gameCamera->view[0][0]);

	
	std::set<GameObject*> staticGO;
	std::set<GameObject*> dynamicGO;

	if (App->scene->quadtreeIsComputed)
	{
		App->scene->quadtree->GetIntersection(staticGO, &gameCamera->frustum->MinimalEnclosingAABB());
	}

	App->scene->aabbTree->GetIntersection(dynamicGO, &gameCamera->frustum->MinimalEnclosingAABB());
	std::set<GameObject*> onCameraGO = staticGO;
	onCameraGO.insert(dynamicGO.begin(), dynamicGO.end());

	for (auto gameObject : onCameraGO)
	{
		if (!gameObject->isEnabled)
			continue;

		if (gameCamera->AABBWithinFrustum(*gameObject->globalBoundingBox) != 0)
		{
			glUniformMatrix4fv(glGetUniformLocation(progModel,
				"model"), 1, GL_TRUE, &gameObject->myTransform->globalModelMatrix[0][0]);

			gameObject->Draw(progModel, true);
			
		}

	}

	glUseProgram(0);
}

void ModuleRender::CreateFrameBuffer(int myWidth, int myHeight, bool scene)
{
	if(scene)
	{
	
		if (myWidth != widthScene || myHeight != heightScene || firstTimeCreatingBuffer)
		{
			if (firstTimeCreatingBuffer)
				firstTimeCreatingBuffer = false;

			if (frameBufferObject == 0)
			{
				//Generate FrameBuffer if necessary
				glCreateFramebuffers(1, &frameBufferObject);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

			if (sceneTexture != 0)
			{
				glDeleteTextures(1, &sceneTexture);
			}

			if (renderBufferObject != 0)
			{
				glDeleteRenderbuffers(1, &renderBufferObject);
			}

			glGenTextures(1, &sceneTexture);
			glBindTexture(GL_TEXTURE_2D, sceneTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myWidth, myHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

			//Generate RenderBuffers
			glGenRenderbuffers(1, &renderBufferObject);
			glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
			(!antialiasing) ? glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, myWidth, myHeight) : glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, myWidth, myHeight);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);


			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				LOG("ERROR: Cannot create or render Scene framebuffer.");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
	else
	{
	
		if (myWidth != widthGame || myHeight != heightGame || firstTimeCreatingBuffer)
		{
			if (frameBufferObjectGame == 0)
			{
				//Generate FrameBuffer if necessary
				glCreateFramebuffers(1, &frameBufferObjectGame);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectGame);

			if (gameTexture != 0)
			{
				glDeleteTextures(1, &gameTexture);
			}

			if (renderBufferObjectGame != 0)
			{
				glDeleteRenderbuffers(1, &renderBufferObjectGame);
			}

			glGenTextures(1, &gameTexture);
			glBindTexture(GL_TEXTURE_2D, gameTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myWidth, myHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gameTexture, 0);

			//Generate RenderBuffers
			glGenRenderbuffers(1, &renderBufferObjectGame);
			glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObjectGame);
			(!antialiasing) ? glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, myWidth, myHeight) : glRenderbufferStorageMultisample(GL_RENDERBUFFER,4, GL_DEPTH24_STENCIL8, myWidth, myHeight);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER,4, GL_DEPTH24_STENCIL8, myWidth, myHeight);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObjectGame);


			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				LOG("ERROR: Cannot create or render Scene framebuffer.");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}


}

void ModuleRender::GenerateTexture(int myWidth, int myHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glViewport(0, 0, myWidth, myHeight);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw all scene
	if(showFrustum)
		App->scene->mainCamera->DrawCamera();

	DrawDebug();
	DrawAllGameObjects();
	
	
	if(skybox != nullptr && showSkybox)
		skybox->DrawSkybox();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Why outside of framebuffer?
	App->debugDraw->Draw(App->camera, frameBufferObject, myWidth, myHeight);
}

void ModuleRender::GenerateTextureGame(int myWidth, int myHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectGame);
	glViewport(0, 0, myWidth, myHeight);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawGame();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRender::Pick() const
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && ImGui::IsWindowFocused() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
	{
		ImVec2 pos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		App->scene->PickObject(size, pos);

	}

	return;
}

void ModuleRender::DrawGuizmoButtons() const
{
	if(ImGui::Button(ICON_FA_HAND_ROCK ""))
	{
		(ImGuizmo::OPERATION)currentOperation = ImGuizmo::TRANSLATE;
	}

	ImGui::SameLine();

	if(ImGui::Button(ICON_FA_SYNC ""))
	{
		(ImGuizmo::OPERATION)currentOperation = ImGuizmo::ROTATE;
	}

	ImGui::SameLine();

	if(ImGui::Button(ICON_FA_BALANCE_SCALE ""))
	{
		(ImGuizmo::OPERATION)currentOperation = ImGuizmo::SCALE;
	}
	ImGui::SameLine();

	if(ImGui::Button(ICON_FA_GLOBE ""))
	{
		(ImGuizmo::MODE)currentMode = ImGuizmo::WORLD;
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_MAP_MARKER ""))
	{
		(ImGuizmo::MODE)currentMode = ImGuizmo::LOCAL;
	}

}

void ModuleRender::DrawDebug() const
{
	if(showQuadTree && App->scene->quadtreeIsComputed)
	{
		//Iterative
		App->scene->quadtree->DrawIterative();
	}

	if(showGrid)
	{
		dd::xzSquareGrid(-40.0f, 40.0f, 0.0f, 1.0f, math::float3(0.65f, 0.65f, 0.65f));
	}

	if(showAABBTree)
	{
		App->scene->aabbTree->Draw();
	}

	return;
}

void ModuleRender::DrawSceneBuffer()
{
	ImGuizmo::BeginFrame();
	bool isEnabled = true;
	//First Scene window is created
	ImGui::SetNextWindowPos(
		ImVec2(App->window->width * App->imgui->scenePosRatioWidth, App->window->height * App->imgui->scenePosRatioHeight)
	);
	ImGui::SetNextWindowSize(
		ImVec2(App->window->width * App->imgui->sceneSizeRatioWidth, App->window->height * App->imgui->sceneSizeRatioHeight)
	);
	ImGui::Begin("Scene", &isEnabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	App->camera->SceneNotActive = ImGui::IsWindowFocused();

	ImVec2 wSize = ImGui::GetWindowSize();
	App->camera->SetAspectRatio((int)wSize.x, (int)wSize.y);

	//Call MousePicking routine
	Pick();

	CreateFrameBuffer((int)wSize.x, (int)wSize.y);
	GenerateTexture((int)wSize.x, (int)wSize.y);

	widthScene = (int)wSize.x;
	heightScene = (int)wSize.y;

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

	DrawGuizmo();


	ImGui::End();

	return;
}

void ModuleRender::DrawGameBuffer()
{
	bool gameIsEnabled = true;

	ImGui::SetNextWindowPos(
		ImVec2(App->window->width * App->imgui->scenePosRatioWidth, App->window->height * App->imgui->scenePosRatioHeight),
		ImGuiCond_Once
	);
	ImGui::SetNextWindowSize(
		ImVec2(App->window->width * App->imgui->sceneSizeRatioWidth, App->window->height * App->imgui->sceneSizeRatioHeight),
		ImGuiCond_Once
	);
	//Game Window
	ImGui::Begin(" Game", &gameIsEnabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 wSizeGame = ImGui::GetWindowSize();

	if (App->scene->mainCamera == nullptr)
	{
		ImGui::End();
		LOG("Main camera is nullptr.");
		return;
	}


	gameCamera->SetAspectRatio((int)wSizeGame.x, (int)wSizeGame.y);
	CreateFrameBuffer((int)wSizeGame.x, (int)wSizeGame.y, false);
	GenerateTextureGame((int)wSizeGame.x, (int)wSizeGame.y);

	widthGame = (int)wSizeGame.x;
	heightGame = (int)wSizeGame.y;

	ImGui::GetWindowDrawList()->AddImage(
		(void *)gameTexture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(
			ImGui::GetCursorScreenPos().x + wSizeGame.x,
			ImGui::GetCursorScreenPos().y + wSizeGame.y
		),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();

	return;
}








void ModuleRender::EnableFaceCulling() const
{
		if (faceCullingIsActive)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
}


//After use a vbo assign a 0 for efficency
void ModuleRender::EnableDepthTest() const
{
	if (dephtTestIsActive)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}


void ModuleRender::ChangeFrontFace() const
{
	if (changingFrontFace)
	{
		glFrontFace(GL_CCW);
	}
	else
	{
		glFrontFace(GL_CW);
	}
}



void ModuleRender::EnableTexture2D() const
{
	if (texture2DIsActive)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void ModuleRender::FillTriangles() const
{
	if (fillTrianglesIsActive)
	{
		glPolygonMode(GL_FRONT, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT, GL_LINE);
	}
}

void ModuleRender::EnableAlphaTest() const
{
	if (alphaTestIsActive)
	{
		glEnable(GL_ALPHA_TEST);
	}
	else
	{
		glDisable(GL_ALPHA_TEST);
	}
}