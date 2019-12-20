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
	//Initialize Texture
	LOG("Init Image library");
	ilInit();
	iluInit();
	ilutInit();

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



void ModuleTexture::LoadSkybox(const char * path, const std::string & directory, int index)const
{
	std::string filepath = directory;
	filepath.append(path);

	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	bool isLoaded1 = ilLoadImage(filepath.c_str());

	if(!isLoaded)
	{
		LOG("ERROR: Cannot load image.");
		return;
	}
	
	//Make sure image is in RGB or devil will return an empty string
	bool converted = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	if (!converted)
	{
		ILenum error = ilGetError();
		LOG("Error converting image to rgb: %s - %s", std::to_string(error), iluErrorString(error));
		return;
	}


	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (!(ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT))
	{
		LOG("Flipping Image Origin");
		iluFlipImage();
	}

	ILubyte* data = ilGetData();

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int heigth = ilGetInteger(IL_IMAGE_HEIGHT);


	if (data)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
			0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);

		//Delete image
		LOG("Delete image");
		ilDeleteImages(1, &image);

	}
	else
	{
		LOG("Cubemap texture failed to load at path: ", filepath.c_str());
		return;
	}

	return;
}

void ModuleTexture::LoadTextureForModels(const char * path, const std::string &directory, Texture &texture)
{
	std::string filepath = directory;
	filepath.append(path);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	LOG("Loading texture %s . \n", filepath.c_str());
	LOG("Creating image");
	//Loading image
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);


	LOG("Trying to load texture from path described on FBX.");
	bool isLoaded1 = ilLoadImage(filepath.c_str());

	if (isLoaded1)
	{
		LOG("Texture found in path described on FBX");
	}
	else
	{
		LOG("Can not find texture in path described on FBX.")

			filepath.clear();
		filepath = path;

		LOG("Trying to load texture from same folder than model.")

			bool isLoaded2 = ilLoadImage(filepath.c_str());

		if (isLoaded2)
		{
			LOG("Texture found in same folder than model.");
		}
		else
		{
			LOG("Can not find texture in same folder than model.")

				filepath.clear();
			std::string filepath = "../Textures/";
			filepath.append(path);

			LOG("Trying to load texture from Textures directory.")

				bool isLoaded3 = ilLoadImage(filepath.c_str());
			if (isLoaded3)
			{
				LOG("Texture found in Textures directory.");
			}
			else
			{
				LOG("Can not find textures");
			}
		}
	}

	//Make sure image is in RGB or devil will return an empty string
	bool converted = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	if (!converted)
	{
		ILenum error = ilGetError();
		LOG("Error converting image to rgb: %s - %s", std::to_string(error), iluErrorString(error));
		return;
	}


	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		LOG("Flipping Image Origin");
		iluFlipImage();
	}

	ILubyte* data = ilGetData();

	if(data)
	{
		//Fill texture
		texture.width = ilGetInteger(IL_IMAGE_WIDTH);
		texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
		texture.depth = ilGetInteger(IL_IMAGE_DEPTH);
		texture.format = ilDetermineType(path);
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
	LOG("Delete image");
	iluDeleteImage(image);



	
	texture.id = textureID;

	return;
}

std::vector<Texture> ModuleTexture::loadMaterialTextures(aiMaterial * mat, const aiTextureType type, const std::string typeName, const std::string &directory)
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

