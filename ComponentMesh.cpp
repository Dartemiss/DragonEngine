#include "ComponentMesh.h"
#include "ModuleModelLoader.h"
#include "Application.h"

ComponentMesh::ComponentMesh()
{
	myType = MESH;
}


ComponentMesh::~ComponentMesh()
{
	for (auto mesh : meshes)
	{
		delete mesh;
	}
}

void ComponentMesh::Update()
{
	return;
}

void ComponentMesh::LoadMeshes(const char * path)
{
	App->modelLoader->loadModel(path, meshes);
	App->modelLoader->emptyScene();
	LOG("GameObject have %d number of meshes.", meshes.size());
}
