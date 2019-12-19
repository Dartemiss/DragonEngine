#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>
#include "Mesh.h"
#include <Assimp/postprocess.h>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include "GL/glew.h"
#include <DevIL/il.h>
#include <DevIL/ilu.h>
#include <DevIL/ilut.h>
#include <Assimp/material.h>





class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader() = default;
	~ModuleModelLoader() = default;

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Draw(const unsigned int program) const;
	void loadModel(const std::string &path);

	const int GetNumberOfMeshes() const;
	const int GetNumberOfTriangles(const bool triangles) const;
	void GetMeshes(std::vector<Mesh*> &meshes);
	bool LoadSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size,
		unsigned slices, unsigned stacks, const math::float4& color);

	bool LoadCylinder(const char* name, const math::float3& pos, const math::Quat& rot, float height,
		float radius, unsigned slices, unsigned stacks, const math::float4& color);
	
	bool LoadTorus(const char* name, const math::float3& pos, const math::Quat& rot, float inner_r, float outer_r,
		unsigned slices, unsigned stacks, const math::float4& color);

	bool LoadCube(const char* name, const math::float3& pos, const math::Quat& rot, float size, const math::float4& color);
	

	//Variables
	//Representation of a Cube, have exactly 8 vertex
	//Order of representation:
	//0-> (-x,-y,-z), 1-> (x,-y,-z), 2-> (x,-y,z), 3-> (-x,-y,z)
	//4-> (-x,y,-z), 5-> (x,y,-z), 6-> (x,y,z), 7-> (-x,y,z)
	std::vector<float3> modelBox;
	bool isModelLoaded = false;
	float3 correctCameraPositionForModel = float3(0.0f, 0.0f, 0.0f);
	float3 modelCenter = float3(0.0f, 0.0f, 0.0f);
	int numberOfTextures = 0;
	void emptyScene();

	std::vector<Mesh*> meshes;
	std::string nameOfModel;

private:

	
	std::string directory;
	/*  Functions   */
	
	void processNode(aiNode* node, const aiScene *scene);
	Mesh* processMesh(const aiMesh* mesh, const aiScene *scene);
	
	std::string computeDirectory(const std::string &path) const;
	std::string ComputeName(const std::string &path) const;





};
#endif __ModuleModelLoader_h__
