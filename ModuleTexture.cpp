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
	//ilutInit();

	LoadTexture("../Textures/Lenna.png");
	LoadTexture("../Textures/g2logo.jpg");
	LoadTexture("../Textures/blackDragon.dds");


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textures[0].Width, textures[0].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, textures[0].Data);
	//Texture
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

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
	for(unsigned int i = 0; i < textures.size();++i)
	{
		glDeleteTextures(1, &textures[i].Id);
	}
	
	return true;
}

void ModuleTexture::LoadTexture(char * path)
{
	//Texture
	unsigned int imageAux = 0;
	imagesTex.push_back(imageAux);
	int size = imagesTex.size();
	ilGenImages(1, &imagesTex[size-1]);
	ilBindImage(imagesTex[size - 1]);

	isLoaded = ilLoadImage(path);

	ILinfo imageInfo;

	imageInfo.Width = ilGetInteger(IL_IMAGE_WIDTH);
	imageInfo.Height = ilGetInteger(IL_IMAGE_HEIGHT);
	imageInfo.Data = ilGetData();
	imageInfo.Depth = ilGetInteger(IL_IMAGE_DEPTH);


	iluGetImageInfo(&imageInfo);
	imageInfo.Format = ilDetermineType(path);
	

	textures.push_back(imageInfo);

	glGenTextures(1, &imageInfo.Id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, imageInfo.Id);
	glUniform1i(glGetUniformLocation(App->renderer->prog, "texture0"), 1);


	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	
	glGenerateMipmap(GL_TEXTURE_2D);

	//ilDeleteImages(1, &imagesTex[size - 1]);
	glBindTexture(GL_TEXTURE_2D, 0);

	return;
}

void ModuleTexture::executeTexImage2D()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->texture->textures[indexTex].Width, App->texture->textures[indexTex].Height, 0, GL_RGB, GL_UNSIGNED_BYTE, App->texture->textures[indexTex].Data);
}
