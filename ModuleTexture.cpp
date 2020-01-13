#include "ModuleTexture.h"
#include "ModuleRender.h"
#include "Application.h"
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
		//TODO: fix error when loading bunny: critical error detected c0000374
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

void ModuleTexture::LoadSkybox(const char * path, const std::string & directory, int index)const
{
	//TODO: Load skybox using material importer

	//std::string filepath = directory;
	//filepath.append(path);

	//ILuint image;
	//ilGenImages(1, &image);
	//ilBindImage(image);

	//bool isLoaded1 = ilLoadImage(filepath.c_str());

	//if(!isLoaded)
	//{
	//	LOG("ERROR: Cannot load image.");
	//	return;
	//}
	//
	////Make sure image is in RGB or devil will return an empty string
	//bool converted = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	//if (!converted)
	//{
	//	ILenum error = ilGetError();
	//	LOG("Error converting image to rgb: %s - %s", std::to_string(error), iluErrorString(error));
	//	return;
	//}


	//ILinfo ImageInfo;
	//iluGetImageInfo(&ImageInfo);
	//if (!ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	//{
	//	LOG("Flipping Image Origin");
	//	iluFlipImage();
	//}

	//ILubyte* data = ilGetData();

	//int width = ilGetInteger(IL_IMAGE_WIDTH);
	//int heigth = ilGetInteger(IL_IMAGE_HEIGHT);


	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
	//		0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	//	);

	//	//Delete image
	//	LOG("Delete image");
	//	ilDeleteImages(1, &image);

	//}
	//else
	//{
	//	LOG("Cubemap texture failed to load at path: ", filepath.c_str());
	//	return;
	//}

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
