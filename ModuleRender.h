#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float3x3.h"
#include "ComponentCamera.h"
#include "GL/glew.h"
#include <vector>
#include <set>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class Skybox;

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


	//Touch render variables
	void EnableFaceCulling() const;
	void EnableDepthTest() const;
	void ChangeFrontFace() const;
	void EnableTexture2D() const;
	void FillTriangles() const;
	void EnableAlphaTest() const;

	bool faceCullingIsActive = true;
	bool dephtTestIsActive = true;
	bool changingFrontFace = true;
	bool texture2DIsActive = true;
	bool fillTrianglesIsActive = true;
	bool alphaTestIsActive = false;

	float aspect = 1.0f;

	float4x4 model = float4x4::zero;

	bool showBoundingBox = true;
	bool showSkybox = false;

	//Frustum Culling
	bool frustumCullingIsActivated = false;


	//Debug
	//void OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	//Draw
	void DrawGrid();
	void DrawAllGameObjects();
	void DrawGame();
	
	//If scene create buffer for scene else create buffer for game window
	void CreateFrameBuffer(int width, int height, bool scene = true);
	void GenerateTexture(int width, int height);
	void GenerateTextureGame(int width, int height);

	//Quadtree variables
	bool showQuadTree = false;
	bool showAABBTree = true;
	bool showFrustum = true;
	bool showGrid = false;

private:
	void* context;

	int height = 0;
	int width = 0;

	unsigned int vbo;
	unsigned int index;

	//Framebuffer windows variables
	unsigned int frameBufferObject = 0; // FBO
	unsigned int renderBufferObject = 0; // RBO

	//Framebuffer windows variables
	unsigned int frameBufferObjectGame = 0; // FBO
	unsigned int renderBufferObjectGame = 0; // RBO

	int heightScene, widthScene;
	int heightGame, widthGame;

	unsigned int sceneTexture = 0;
	unsigned int gameTexture = 0;
	
	ComponentCamera* gameCamera = nullptr;
	
	std::vector<GameObject*> gameObjectsWithinFrustum;

	//Skybox
	Skybox* skybox = nullptr;

	//Methods
	void DrawDebug() const;
	void DrawSceneBuffer();
	void DrawGameBuffer();
	

};

#endif __ModuleRender_H__