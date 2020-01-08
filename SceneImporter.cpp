#include "SceneImporter.h"

#include "ModelImporter.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"

using namespace std;

SceneImporter::SceneImporter()
{
	modelImp = new ModelImporter();
	meshImp = new MeshImporter();
	materialImp = new MaterialImporter();
}

SceneImporter::~SceneImporter()
{
	delete modelImp;
	delete meshImp;
	delete materialImp;
}

bool SceneImporter::ImportModel(const char * path, const char * file, string & output_file)
{
	return modelImp->Import(path, file, output_file);
}

bool SceneImporter::ImportMesh(const char * file, const MeshData & mesh, string & output_file)
{
	return meshImp->Import(file, mesh, output_file);
}

bool SceneImporter::ImportMaterial(const char * path, const char * file, string & output_file)
{
	return materialImp->Import(path, file, output_file);
}


bool SceneImporter::ImportModel(const char * file, const void * buffer, unsigned int size, string & output_file)
{
	return modelImp->Import(file, buffer, size, output_file);
}

bool SceneImporter::ImportMesh(const char * file, const void * buffer, unsigned int size, string & output_file)
{
	return meshImp->Import(file, buffer, size, output_file);
}

bool SceneImporter::ImportMaterial(const char * file, const void * buffer, unsigned int size, string & output_file)
{
	return materialImp->Import(file, buffer, size, output_file);
}


bool SceneImporter::LoadModel(const char * exported_file, ModelData & model)
{
	return modelImp->Load(exported_file, model);
}

bool SceneImporter::LoadMesh(const char * exported_file, MeshData & mesh)
{
	return meshImp->Load(exported_file, mesh);
}

bool SceneImporter::LoadMaterial(const char * exported_file, Texture & material)
{
	return materialImp->Load(exported_file, material);
}
