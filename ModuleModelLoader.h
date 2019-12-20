#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <map>
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

struct Texture;

struct Model
{
	std::map<Mesh*, Texture*> Meshes;
	std::string Name = "";
	std::string Directory = "";
	//TODO: add count of similar models to track delete and remove data?
};

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

	void LoadModel(const std::string &path, Model& model);

	const int GetNumberOfMeshes() const;
	bool LoadSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size,
		unsigned slices, unsigned stacks, const math::float4& color);

	bool LoadCylinder(const char* name, const math::float3& pos, const math::Quat& rot, float height,
		float radius, unsigned slices, unsigned stacks, const math::float4& color);
	
	bool LoadTorus(const char* name, const math::float3& pos, const math::Quat& rot, float inner_r, float outer_r,
		unsigned slices, unsigned stacks, const math::float4& color);

	bool LoadCube(const char* name, const math::float3& pos, const math::Quat& rot, float size, const math::float4& color);
	

	float3 correctCameraPositionForModel = float3(0.0f, 0.0f, 0.0f);
	float3 modelCenter = float3(0.0f, 0.0f, 0.0f);

	std::vector<Model> models;

private:

	/*  Functions   */
	
	void ProcessNode(aiNode* node, const aiScene *scene, Model &model);
	Mesh* ProcessMesh(const aiMesh* mesh, const aiScene *scene);
	void ProcessTextures(const aiMesh *mesh, const aiScene *scene, const std::string &directory, std::vector<Texture> &textures);

	std::string ComputeDirectory(const std::string &path) const;
	std::string ComputeName(const std::string &path) const;

};

#endif __ModuleModelLoader_h__