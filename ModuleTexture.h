#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <string>

class Application;

struct Texture {
	unsigned int id = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;
	unsigned int format = 0;
	void* data = nullptr;
	std::string type = "";
	std::string path = "";  // we store the path of the texture to compare with other textures
};


class ModuleTexture : public Module
{

public:
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void LoadTexture(Texture & texture);
	void LoadSkybox(const char* path, int index, unsigned int &image) const;
	void LoadWhiteFallbackTexture();

	Texture * getWhiteFallbackTexture();

	//Vector of all the textures that have been loaded for avoiding reimporting
	std::vector<Texture> textures_loaded;
	Texture white_fallback;
};
#endif __ModuleTexture_H__
