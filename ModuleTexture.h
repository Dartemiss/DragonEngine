#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>
#include "ilu.h"

class Application;

struct Texture {
	unsigned int id = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;
	unsigned int format = 0;
	unsigned char* data = nullptr;
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

	void LoadTexture(char* path, Texture* textureLoaded, ILuint &image);

	//Vector of all the textures that have been loaded for avoiding reimporting
	std::vector<Texture> textures_loaded;

	unsigned int indexTex = 0;
	void executeTexImage2D();
	bool isLoaded;


};
#endif __ModuleTexture_H__
