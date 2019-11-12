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

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Draw(unsigned int program);
	void loadModel(const std::string path);

private:

	std::vector<Mesh*> meshes;
	std::string directory;
	/*  Functions   */
	
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	
	std::string computeDirectory(const std::string path);

	unsigned int TextureFromFile(const char *path, const std::string &directory, Texture* texture, bool gamma = false);
	bool isModelLoaded = false;
	void emptyScene();
};
#endif __ModuleModelLoader_h__
