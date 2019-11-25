#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>
#include "Mesh.h"
#include <postprocess.h>
#include <Importer.hpp>
#include <scene.h>
#include "glew.h"
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <material.h>
#include <set>


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
	void AddTextureIndex(const std::vector<Texture> &textures);
	void ChangeTexture(const std::string &path);

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

	//Indices cannot be repeated (Set)
	std::set<unsigned int> indicesOfCurrentTextures;

	//Assimp transformation
	aiVector3D pos;
	aiQuaternion rot;
	aiVector3D scale;

private:

	std::vector<Mesh*> meshes;
	std::string directory;

	/*  Functions   */
	
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	
	std::string computeDirectory(const std::string &path) const;

	void emptyScene();

	void computeModelBoundingBox();


	



};
#endif __ModuleModelLoader_h__
