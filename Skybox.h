#ifndef __Skybox_H__
#define __Skybox_H__

#include "Globals.h"
#include <vector>
#include <string>

class Skybox
{
public:
	Skybox();
	~Skybox();

	unsigned int LoadCubeMap(const std::vector<std::string> &faces);
	void DrawSkybox() const;
	std::string directory = "";

	unsigned int cubemapTexture = 0;

	unsigned int skyboxVAO = 0;
	unsigned int skyboxVBO = 0;
	
};

#endif __Skybox_H__
