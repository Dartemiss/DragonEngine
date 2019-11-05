#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include "ilu.h"

class Application;


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

	void LoadTexture(char* path);

	std::vector<ILinfo> textures;
	std::vector<unsigned int> imagesTex;

	unsigned int indexTex = 0;

	bool isLoaded;


};
#endif __ModuleTexture_H__
