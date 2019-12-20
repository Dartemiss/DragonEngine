#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>
#include "DevIL/ilu.h"
#include <Assimp/postprocess.h>
#include <Assimp/material.h>
#include <Assimp/mesh.h>

class Application;

struct Texture {
	unsigned int id = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;
	unsigned int format = 0;
	ILubyte* data = nullptr;
	std::string type = "";
	std::string path = "";  // we store the path of the texture to compare with other textures
};


class ModuleTexture : public Module
{

public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void LoadSkybox(const char* path, const std::string &directory, int index) const;
	void LoadTextureForModels(const char* path, const std::string &directory, Texture &texture);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, const aiTextureType type,
		const std::string typeName, const std::string &directory);

	//Vector of all the textures that have been loaded for avoiding reimporting
	std::vector<Texture> textures_loaded;

	unsigned int indexTex = 0;
	bool isLoaded;


};
#endif __ModuleTexture_H__
