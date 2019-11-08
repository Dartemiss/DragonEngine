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
		delete &textures_loaded[i];
	}
	
	return true;
}

void ModuleTexture::LoadTexture(char * path, Texture* textureLoaded, ILuint image)
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

void ModuleTexture::executeTexImage2D()
{
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->texture->textures[indexTex].Width, App->texture->textures[indexTex].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, App->texture->textures[indexTex].Data);
}
