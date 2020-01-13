#include "ModuleTexture.h"
#include "ModuleRender.h"
#include "Application.h"
#include "SceneImporter.h"
#include "GL/glew.h"

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	for(unsigned int i = 0; i < textures_loaded.size();++i)
	{
		glDeleteTextures(1, &textures_loaded[i].id);
	}
	textures_loaded.clear();

	return true;
}

void ModuleTexture::LoadTexture(Texture & texture)
{
	for (unsigned int i = 0; i < textures_loaded.size(); i++)
	{
		if (strcmp(textures_loaded[i].path.data(), texture.path.data()) == 0)
		{
			texture = textures_loaded[i];
			return;
		}
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (texture.data)
	{
		//Binding texture and generating mipmaps
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG("Failed to load texture.");
	}

	//Filtering and Wrapping texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture.id = textureID;
}

void ModuleTexture::LoadSkybox(const char * path, const std::string & directory, int index) const
{
	Texture skybox;
	Importer->LoadMaterial(path, skybox);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGBA, skybox.width, skybox.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, skybox.data);

	return;
}

void ModuleTexture::LoadWhiteFallbackTexture()
{
	float pixels[] = {1.0f, 1.0f, 1.0f};

	unsigned int whiteTex;

	glGenTextures(1, &whiteTex);
	glBindTexture(GL_TEXTURE_2D, whiteTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, pixels);

	white_fallback.id = whiteTex;
	white_fallback.width = 1;
	white_fallback.height = 1;
	white_fallback.depth = 1;
	//white_fallback.format = 0;
	//white_fallback.data = fallbackImage;
	white_fallback.type = "white_fallback";
	white_fallback.path = "";

	LOG("White for texture fallback loaded.");
}

Texture* ModuleTexture::getWhiteFallbackTexture()
{
	return &white_fallback;
}
