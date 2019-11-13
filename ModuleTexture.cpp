#include "ModuleTexture.h"
#include "ModuleRender.h"
#include "Application.h"
#include "glew.h"
#include <il.h>
#include <ilu.h>
#include <ilut.h>


ModuleTexture::ModuleTexture()
{
}


ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	//Texture
	//Initialize Texture
	LOG("Init Image library");
	ilInit();
	iluInit();
	ilutInit();

	/*
	LoadTexture("../Textures/Lenna.png");
	LoadTexture("../Textures/g2logo.jpg");
	LoadTexture("../Textures/blackDragon.dds");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textures[0].Width, textures[0].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, textures[0].Data);
	//Texture
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	*/
	return true;
}

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

void ModuleTexture::LoadTexture(char * path, Texture* textureLoaded, ILuint &image)
{
	//Texture
	
	ilGenImages(1,&image);
	ilBindImage(image);
	ilLoadImage(path);

	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	textureLoaded->width = ilGetInteger(IL_IMAGE_WIDTH);
	textureLoaded->height = ilGetInteger(IL_IMAGE_HEIGHT);
	textureLoaded->depth = ilGetInteger(IL_IMAGE_DEPTH);
	textureLoaded->format = ilDetermineType(path);;
	textureLoaded->data = (unsigned char*)ilGetData();
	

	return;
}

void ModuleTexture::LoadTextureForModels(const char * path, const std::string directory, Texture &texture)
{
	std::string filepath = directory;
	filepath.append(path);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	LOG("Loading texture %s . \n", filepath);

	//Loading image
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(filepath.c_str());
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	ILubyte* data = ilGetData();

	if(data)
	{
		//Fill texture
		texture.width = ilGetInteger(IL_IMAGE_WIDTH);
		texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
		texture.format = ilDetermineType(path);;
		texture.data = data;

		//Binding texture and generating mipmaps
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG("Failed to load texture");
	}
	//Filtering and Wrapping texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Delete image
	iluDeleteImage(image);

	//Fill shader render with texture name
	//GLuint textureOutput = glGetUniformLocation(App->renderer->progModel,
		//"myTexture");
	//glUniform1i(textureOutput, textureID);

	
	texture.id = textureID;

	return;
}

std::vector<Texture> ModuleTexture::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, std::string directory)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		aiTextureMapping mapping = aiTextureMapping_UV;
		mat->GetTexture(type, i, &str, &mapping, 0);
		bool skip = false;
		for (unsigned int j = 0; j < App->texture->textures_loaded.size(); j++)
		{
			if (std::strcmp(App->texture->textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(App->texture->textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			// if texture hasn't been loaded already, load it
			Texture texture;
			LoadTextureForModels(str.C_Str(), directory, texture);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			App->texture->textures_loaded.push_back(texture); //adding new texture to texture loaded
		}


	}
	return textures;
}

void ModuleTexture::executeTexImage2D()
{
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->texture->textures[indexTex].Width, App->texture->textures[indexTex].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, App->texture->textures[indexTex].Data);
}
