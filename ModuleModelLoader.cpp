#include "ModuleModelLoader.h"
#include "Application.h"
#include <cimport.h>
#include <postprocess.h>
#include <scene.h>
#include <material.h>
#include <mesh.h>


ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
	const aiScene* scene = aiImportFile("../Models/baker_house/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	if(scene == NULL)
	{
		LOG("%s",aiGetErrorString());
	}





	return true;
}

update_status ModuleModelLoader::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleModelLoader::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleModelLoader::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{
	return false;
}
