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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textures[0].TexWidth, textures[0].TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textures[0].TexData);
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
		glDeleteTextures(1,&textures[i].TexId);
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

	TexInfo texInfo;

	texInfo.TexWidth = ilGetInteger(IL_IMAGE_WIDTH);
	texInfo.TexHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	texInfo.TexData = ilGetData();
	

	textures.push_back(texInfo);

	glGenTextures(1, &texInfo.TexId);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texInfo.TexId);
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
